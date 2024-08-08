import praw
import random
import time
import getpass

USERNAME = ""
PASSWORD = ""

CLIENT_ID = ""
CLIENT_SECRET = ""

reddit = praw.Reddit(user_agent="Minesweeper bot (by u/YunusEmre0037)",
                     client_id=CLIENT_ID, client_secret=CLIENT_SECRET,
                     username=USERNAME, password=PASSWORD)

def generateGame(mode):
    game = list()
    mode = mode.lower()
    mine_char = 'o'
    width = 0
    height = 0
    mines = 0
    if mode not in ('easy', 'normal', 'hard'):
        return "error"
    else:
        if mode == 'easy': # 5 x 7, 6 mines
            width = 5
            height = 7
            mines = 6
        elif mode == 'normal': # 6 x 8, 9 mines
            width = 6
            height = 8
            mines = 9
        elif mode == 'hard': # 7 x 10, 14 mines
            width = 7
            height = 10
            mines = 14
    game = [[0 for j in range(width)] for i in range(height)]
    for mine in range(mines):
        while True:
            x = random.randrange(0, width)
            y = random.randrange(0, height)
            if game[y][x] != mine_char:
                game[y][x] = mine_char
                for i in range(0, 3):
                    for j in range(0, 3):
                        try:
                            dx = j - 1
                            dy = i - 1
                            if (dx != 0 or dy != 0) and (y + dy >= 0 and x + dx >= 0) and (y + dy < height and x + dx < width):
                                if game[y + dy][x + dx] != mine_char:
                                    game[y + dy][x + dx] += 1
                        except:
                            pass
                break
    #print(np.array(game))
    safe_spot = None
    while True:
        x = random.randrange(0, width)
        y = random.randrange(0, height)
        if game[y][x] == 0:
            safe_spot = (x, y)
            break
    return (game, safe_spot, width, height, mines)

default_text = "\n\n^Beep ^Boop ^I'm ^a ^bot. ^If ^you ^have ^suggestions ^please ^contact ^to u/YunusEmre0037"

while True:
    for mention in reddit.inbox.mentions():
        try:
            if mention.new:
                game_mode = mention.body.replace('u/MinesweeperBot', '').split()
                if len(game_mode) == 0:
                    c = reddit.comment(mention.id)
                    c.reply("Avalible game modes: easy, normal, hard " + default_text)
                    mention.mark_read()
                    print("replied!")
                    continue

                for word in game_mode:
                    if word.lower() in ('easy', 'normal', 'hard'):
                        game_mode = word
                        break
                my_game = generateGame(game_mode)
                if my_game == 'error':
                    c = reddit.comment(mention.id)
                    c.reply("An error occured!" + default_text)
                    mention.mark_read()
                else:
                    board, safe_spot, w, h, m = my_game
                    reply_text = ''
                    for y in range(h):
                        for x in range(w):
                            if x == safe_spot[0] and y == safe_spot[1]:
                                reply_text += str(board[y][x]) + '\t\t'
                            else:
                                reply_text += '>!{}!<\t\t'.format(board[y][x])
                        reply_text += '\n\n'
                    c = reddit.comment(mention.id)
                    info_text = "Selected game mode: {}\n\nSize: {}x{}\n\nMines: {} ('o' characters represents mines)\n\nGood luck!\n\n".format(game_mode.title(), w, h, m)

                    c.reply(info_text + reply_text + default_text)
                    mention.mark_read()
        except:
            mention.mark_read()

    time.sleep(3)
