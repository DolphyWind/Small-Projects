module Program where

import CommandParser qualified as C
import Control.Monad.State
import System.IO
import qualified Control.Monad.Trans.State as ST

data QuitState = NoQuit | HasChanges | QuitForcefully deriving (Show, Eq, Enum)

data Program = Program
  { getBuffer :: [String],
    getCurrentLine :: Int,
    getFilename :: Maybe FilePath,
    hasUnsavedchanges :: Bool,
    getQuitState :: QuitState,
    getLastError :: String
  }

defaultProgram :: Program
defaultProgram = Program [] 0 Nothing False NoQuit ""

type ProgramStateT = ST.StateT Program IO

sliceToIndex :: Program -> Int -> Int
sliceToIndex (Program buf cl _ _ _ _) x
  | x == C.currentLine = cl - 1
  | x == C.lastLine = length buf - 1
  | otherwise = x - 1

slicesToIndices :: Program -> Int -> Int -> (Int, Int)
slicesToIndices p x y = (sliceToIndex p x, sliceToIndex p y)

sliceList :: [a] -> Int -> Int -> [a]
sliceList l beg end = take (end - beg + 1) $ drop beg l

takeInput :: IO String
takeInput = do
    input <- getLine
    if input /= "." then do
        i2 <- takeInput
        return $ input ++ "\n" ++ i2
    else return ""

runCommand :: C.Command -> ProgramStateT ()
runCommand (C.Plus n) = do
    (Program buf cl fname chg qs lastError) <- get
    if (cl + n) > length buf then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        put $ Program buf (cl + n) fname chg NoQuit lastError
        liftIO . putStrLn $ buf !! (cl + n)

runCommand (C.Minus n) = do
    (Program buf cl fname chg qs lastError) <- get
    if (cl - n) < 1 then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        put $ Program buf (cl - n) fname chg NoQuit lastError
        liftIO . putStrLn $ buf !! (cl - n)

runCommand C.File = do
    p@(Program buf cl fname chg qs lastError) <- get
    case fname of
        Nothing -> do
            put $ Program buf cl fname chg NoQuit "No current filename"
            liftIO $ putStrLn "?"
        (Just f) -> do
            put $ Program buf cl fname chg NoQuit lastError
            liftIO $ putStrLn f

runCommand (C.SetFile path) = do
    (Program buf cl fname chg qs lastError) <- get
    put $ Program buf cl (Just path) chg NoQuit lastError
    liftIO $ putStrLn path

runCommand C.Write = do
    (Program buf cl fname chg qs lastError) <- get
    case fname of
        Nothing -> do
            put $ Program buf cl fname chg NoQuit "No current filename"
            liftIO $ putStrLn "?"
        (Just filename) -> do
            let str = foldr (\a acc -> a ++ "\n" ++ acc) "" buf
            let bytes = length str
            liftIO $ do
                writeFile filename str
                print bytes
            put $ Program buf cl fname False NoQuit lastError

runCommand (C.WriteFile path) = do
    (Program buf cl fname chg qs lastError) <- get
    put $ Program buf cl (Just path) chg NoQuit lastError
    runCommand C.Write

runCommand C.Quit = do
    (Program buf cl fname chg qs lastError) <- get
    if qs == HasChanges then
        put $ Program buf cl fname chg QuitForcefully lastError
    else
        if chg then do
            liftIO $ putStrLn "?"
            put $ Program buf cl fname True HasChanges "Warning: buffer modified"
        else do
            put $ Program buf cl fname True QuitForcefully lastError

runCommand C.ForceQuit = do
    (Program buf cl fname chg qs lastError) <- get
    put $ Program buf cl fname chg QuitForcefully lastError

runCommand (C.Slice beg end C.Print) = do
    p@(Program buf cl fname chg qs lastError) <- get
    let (sbeg, send) = slicesToIndices p beg end
    if sbeg > send || send >= length buf then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        put $ Program buf (send + 1) fname chg NoQuit lastError
        liftIO . mapM_ putStrLn $ sliceList buf sbeg send

runCommand (C.Slice beg end C.LineNumber) = do
    p@(Program buf cl fname chg qs lastError) <- get
    let (sbeg, send) = slicesToIndices p beg end
    if sbeg > send || send >= length buf then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        put $ Program buf (send + 1) fname chg NoQuit lastError
        liftIO $ mapM_ putStrLn $ enumerateLines sbeg send $ sliceList buf sbeg send
    where enumerateLines _ _ [] = []
          enumerateLines sbeg send (x:xs) = (show (sbeg + 1) ++ "\t"++x):enumerateLines (sbeg + 1) send xs

runCommand (C.Slice beg end C.Line) = do
    p@(Program buf cl fname chg qs lastError) <- get
    let (sbeg, send) = slicesToIndices p beg end
    if sbeg > send || send >= length buf then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        put $ Program buf (send + 1) fname chg NoQuit lastError
        liftIO $ mapM_ (putStrLn . (++"$")) $ sliceList buf sbeg send

runCommand (C.Slice beg end C.Append) = do
    p@(Program buf cl fname chg qs lastError) <- get
    let (sbeg, send) = slicesToIndices p beg end
    if sbeg > send || send >= length buf then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        inputLines <- liftIO takeInput
        let newText = lines inputLines
        let newBuf = take (send + 1) buf ++ newText ++ drop (send + 1) buf
        put $ Program newBuf (send + 1 + length newText) fname True NoQuit lastError

runCommand (C.Slice beg end C.Insert) = do
    p@(Program buf cl fname chg qs lastError) <- get
    let (sbeg, send) = slicesToIndices p beg end
    if sbeg > send || send >= length buf then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        inputLines <- liftIO takeInput
        let newText = lines inputLines
        let newBuf = take send buf ++ newText ++ drop send buf
        put $ Program newBuf (send + 1) fname True NoQuit lastError

runCommand (C.Slice beg end C.Change) = do
    runCommand (C.Slice beg end C.Delete)
    runCommand (C.Slice beg beg C.Insert)

runCommand (C.Slice beg end C.Delete) = do
    p@(Program buf cl fname chg NoQuit lastError) <- get
    let (sbeg, send) = slicesToIndices p beg end
    if sbeg > send || send >= length buf then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        put $ Program (take sbeg buf ++ drop (send + 1) buf) (sbeg + 1) fname True NoQuit lastError

runCommand (C.Num n) = do
    p@(Program buf cl fname chg qs lastError) <- get
    let sn = sliceToIndex p n

    if (sn < 0) || (sn >= length buf) then do
        put $ Program buf cl fname chg NoQuit "Invalid address"
        liftIO $ putStrLn "?"
    else do
        put $ Program buf (sn + 1) fname chg NoQuit lastError
        liftIO . putStrLn $ buf !! sn

runCommand C.LastError = do
    (Program buf cl fname chg qs lastError) <- get
    if null lastError then
        return ()
    else do
        liftIO $ putStrLn lastError
    put $ Program buf cl fname chg NoQuit lastError

