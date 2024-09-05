module Main where

import qualified CommandParser as C
import qualified Program as P
import qualified Control.Monad.Trans.State as ST
import System.Environment
import System.Directory

runProgram :: P.Program -> IO ()
runProgram p@(P.Program buf cl fname chg qs lastError) = do
    line <- getLine
    let command = C.runParser C.parseCommand line
    case command of
        Nothing -> do
            putStrLn "?"
            runProgram $ P.Program buf cl fname chg qs "Invalid command suffix"
        (Just (rest, cmd)) -> do
            if null rest then do
                (_, next) <- ST.runStateT (P.runCommand cmd) p
                if P.getQuitState next == P.QuitForcefully then return ()
                else runProgram next
            else do
                putStrLn "?"
                runProgram $ P.Program buf cl fname chg qs "Invalid command suffix"

main :: IO ()
main = do
    args <- getArgs
    if null args then
        runProgram P.defaultProgram
    else do
        let fname = head args
        e <- doesFileExist fname
        if e then do
            content <- readFile fname
            let bytes = length content
            let buf = lines content
            print bytes
            runProgram $ P.Program buf (length buf) (Just fname) False P.NoQuit ""
        else do
            putStr fname
            putStrLn " No such file or directory"
            runProgram $ P.Program [] 0 Nothing False P.NoQuit "Cannot open input file"
            
