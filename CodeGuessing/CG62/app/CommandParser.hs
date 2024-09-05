module CommandParser (
    Command(..),
    Action(..),
    Parser,
    parseCommand,
    currentLine,
    lastLine,
    runParser
) where

import Control.Applicative
import Data.Char (isNumber)

data Command =
             Plus Int
           | Minus Int
           | File
           | SetFile FilePath
           | Write
           | WriteFile FilePath
           | Quit
           | ForceQuit
           | Slice { getSliceBegin :: Int, getSliceEnd :: Int, getSliceAction :: Action}
           | Num Int
           | LastError
    deriving (Show, Eq)

data Action =
            Print
          | LineNumber
          | Line
          | Append
          | Insert
          | Change
          | Delete
    deriving (Show, Eq)

newtype Parser a = Parser { runParser :: String -> Maybe (String, a) }

instance Functor Parser where
    fmap :: (a -> b) -> Parser a -> Parser b
    fmap f (Parser p) = Parser $ \s -> case p s of
        Nothing -> Nothing
        (Just (rest, x)) -> Just (rest, f x)

instance Applicative Parser where
    pure :: a -> Parser a
    pure x = Parser $ \s -> Just (s, x)

    (<*>) :: Parser (a -> b) -> Parser a -> Parser b
    (Parser pf) <*> (Parser pa) = Parser $ \s -> do
        (rest, f) <- pf s
        (rest', a) <- pa rest
        return (rest', f a)

instance Alternative Parser where
    empty :: Parser a
    empty = Parser $ const Nothing

    (<|>) :: Parser a -> Parser a -> Parser a
    (Parser p1) <|> (Parser p2) = Parser $ \s -> p1 s <|> p2 s

parseChar :: Char -> Parser Char
parseChar c = Parser f
    where f [] = Nothing
          f (x:xs)
              | x == c = Just (xs, c)
              | otherwise = Nothing

parseCommandHelper :: Char -> Command -> (Char -> Command)
parseCommandHelper c cmd = \ch -> if ch == c then cmd else undefined

parseActionHelper :: Char -> Action -> (Char -> Action)
parseActionHelper c act = \ch -> if ch == c then act else undefined

currentLine :: Int
currentLine = -2

lastLine :: Int
lastLine = -1

parseQuit :: Parser Command
parseQuit = parseCommandHelper 'q' Quit <$> parseChar 'q'

parseForceQuit :: Parser Command
parseForceQuit = parseCommandHelper 'Q' ForceQuit <$> parseChar 'Q'

parseLastError :: Parser Command
parseLastError = parseCommandHelper 'h' LastError <$> parseChar 'h'

parseWriteFile :: Parser Command
parseWriteFile = f <$> (parseChar 'w' *> parseWs *> parseNonWs)
    where f [] = Write
          f str = WriteFile str

parseSetFile :: Parser Command
parseSetFile = f <$> (parseChar 'f' *> parseWs *> parseNonWs)
    where f [] = File
          f str = SetFile str

parsePlusNum :: Parser Command
parsePlusNum = Plus <$> (parseChar '+' *> parseWs *> parseNumber)

parsePlusses :: Parser Command
parsePlusses = Plus . (+1) . length <$> (parseChar '+' *> spanP (=='+'))

parseMinusNum :: Parser Command
parseMinusNum = Minus <$> (parseChar '-' *> parseWs *> parseNumber)

parseMinuses :: Parser Command
parseMinuses = Minus . (+1) . length <$> (parseChar '-' *> spanP (=='-'))

parseComma :: Parser Command
parseComma = Parser $ \s -> do
    (rest, num1) <- runParser (parseNumber <|> pure 1) s
    (rest', _) <- runParser (parseWs *> parseChar ',') rest
    (rest'', num2) <- runParser (parseNumber <|> pure lastLine) rest'
    (rest''', act) <- runParser (parseWs *> parseAction) rest''

    return (rest''', Slice num1 num2 act)

parseSemicolon :: Parser Command
parseSemicolon = Parser $ \s -> do
    (rest, num1) <- runParser (parseNumber <|> pure currentLine) s
    (rest', _) <- runParser (parseWs *> parseChar ';') rest
    (rest'', num2) <- runParser (parseNumber <|> pure lastLine) rest'
    (rest''', act) <- runParser (parseWs *> parseAction) rest''

    return (rest''', Slice num1 num2 act)

parseSingleSlice :: Parser Command
parseSingleSlice = Parser $ \s -> do
    (rest, num) <- runParser parseNumber s
    (rest', act) <- runParser (parseWs *> parseAction) rest

    return (rest', Slice num num act)

parseNumberCommand :: Parser Command
parseNumberCommand = Num <$> parseNumber

parsePrint :: Parser Action
parsePrint = parseActionHelper 'p' Print <$> parseChar 'p'

parseDelete :: Parser Action
parseDelete = parseActionHelper 'd' Delete <$> parseChar 'd'

parseLine :: Parser Action
parseLine = parseActionHelper 'l' Line <$> parseChar 'l'

parseAppend :: Parser Action
parseAppend = parseActionHelper 'a' Append <$> parseChar 'a'

parseInsert :: Parser Action
parseInsert = parseActionHelper 'i' Insert <$> parseChar 'i'

parseChange :: Parser Action
parseChange = parseActionHelper 'c' Change <$> parseChar 'c'

parseLineNumber :: Parser Action
parseLineNumber = parseActionHelper 'n' LineNumber <$> parseChar 'n'

stringToInt :: String -> Int
stringToInt s
    | s == "$" = lastLine
    | s == "." = currentLine
    | otherwise = read s

isNumberPred :: Char -> Bool
isNumberPred c
    | c == '.' = True
    | c == '$' = True
    | otherwise = isNumber c

parseNumber :: Parser Int
parseNumber = Parser $ \s ->
    case span isNumberPred s of
        ("", _) -> Nothing
        (digits, rest) -> Just (rest, stringToInt digits)


spanP :: (Char -> Bool) -> Parser String
spanP f =
  Parser $ \input -> do
    let (str, rest) = span f input
    return (rest, str)

isWhiteSpace :: Char -> Bool
isWhiteSpace c
    | c == ' ' = True
    | c == '\t' = True
    | otherwise = False

parseWs :: Parser String
parseWs = spanP isWhiteSpace

parseNonWs :: Parser String
parseNonWs = spanP (not . isWhiteSpace)

parseAction :: Parser Action
parseAction = parsePrint <|> parseDelete <|> parseLine <|> parseAppend <|> parseInsert <|> parseChange <|> parseLineNumber

makeSlice :: Action -> Command
makeSlice = Slice currentLine currentLine

parseActionCommand :: Parser Command
parseActionCommand = makeSlice <$> parseAction

parseCommand :: Parser Command
parseCommand = parseWs *> (
                 parseQuit <|> parseForceQuit <|> parseActionCommand <|> parseWriteFile <|>
                 parseSetFile <|> parsePlusNum <|> parsePlusses <|> parseMinusNum <|>
                 parseMinuses <|> parseComma <|> parseSemicolon <|> parseSingleSlice <|>
                 parseNumberCommand <|> parseLastError
               ) <* parseWs
