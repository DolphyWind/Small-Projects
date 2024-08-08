import antigravity # :O
from copy import deepcopy as shallow_copy # >:D

MALE = 0
ASEXUAL = 1
FEMALE = 2
POLYGENDER = 3

CIS = 0
TRANS = 1

HE_HIM = 0
SHE_HER = 1
THEY_THEM = 2

def transition(gender):
    return (gender + 2) % 4

class Queer:
    def __init__(self, femininity, masculinity, t) -> None:
        self.femininity = femininity
        self.masculinity = masculinity
        
        self.down_gender_identity = int(POLYGENDER in femininity)
        self.up_gender_identity = int(ASEXUAL in femininity)
        self.left_gender_identity = int(FEMALE in femininity)
        self.right_gender_identity = int(MALE in femininity)

        self.pronouns = t

plus_1 = Queer([POLYGENDER, ASEXUAL], [FEMALE, MALE], HE_HIM)
plus_2 = Queer([FEMALE, MALE], [POLYGENDER, ASEXUAL], HE_HIM)

forward_1 = Queer([FEMALE, ASEXUAL], [MALE, POLYGENDER], SHE_HER)
forward_2 = Queer([MALE, POLYGENDER], [FEMALE, ASEXUAL], SHE_HER)

backward_1 = Queer([MALE, ASEXUAL], [FEMALE, POLYGENDER], THEY_THEM)
backward_2 = Queer([FEMALE, POLYGENDER], [MALE, ASEXUAL], THEY_THEM)

# Wave function collapse!
class Tile:
    def __init__(self, grid) -> None:
        self.possible_genders = []
        self.gender = None
        self.grid = grid
        self.x = 0
        self.y = 0

        self.gay_checks = [False, False]

    def collapse(self, abusive_parents=False):
        if self.gender:
            return
        if len(self.possible_genders) == 0:
            return

        genders = self.possible_genders
        right = None if self.x == len(self.grid[0]) - 1 else self.grid[self.y][self.x + 1]
        down = None if self.y == len(self.grid) - 1 else self.grid[self.y + 1][self.x]
        left = None if self.x == 0 else self.grid[self.y][self.x - 1]
        up = None if self.y == 0 else self.grid[self.y - 1][self.x]

        if abusive_parents:
            self.gender = self.possible_genders[0]
            if left:
                left.collapse()
            if right:
                right.collapse()
            if up:
                up.collapse()
            if down:
                down.collapse()
            return

        if left and left.gender:
            genders = [gender for gender in genders if gender.left_gender_identity == left.gender.right_gender_identity]
        if right and right.gender:
            genders = [gender for gender in genders if gender.right_gender_identity == right.gender.left_gender_identity]
        if up and up.gender:
            genders = [gender for gender in genders if gender.up_gender_identity == up.gender.down_gender_identity]
        if down and down.gender:
            genders = [gender for gender in genders if gender.down_gender_identity == down.gender.up_gender_identity]    

        self.possible_genders = genders
        if len(genders) == 0:
            raise Exception("B-but y-you said t-that all entries w-would be va-valid :(")
        if len(genders) == 1:
            self.gender = genders[0]
            if left:
                left.collapse()
            if right:
                right.collapse()
            if up:
                up.collapse()
            if down:
                down.collapse()

    def make_possible_genders(self, strgrid, x, y):
        self.x = x
        self.y = y

        if strgrid[y][x] == '+':
            self.possible_genders.append(plus_1)
            self.possible_genders.append(plus_2)
        elif strgrid[y][x] == '/':
            self.possible_genders.append(forward_1)
            self.possible_genders.append(forward_2)
        elif strgrid[y][x] == '\\':
            self.possible_genders.append(backward_1)
            self.possible_genders.append(backward_2)


def get_next_direction_to_move(tile, current_dir, color):
    inv_dir = transition(current_dir)
    dirs = []
    if color == TRANS:
        dirs = shallow_copy(tile.gender.femininity)
    else:
        dirs = shallow_copy(tile.gender.masculinity)
    
    dirs = [d for d in dirs if d != inv_dir]
    return dirs[0]

def get_wall_hit(grid, current_dir, color, x, y, w, h):
    i_cant_find_name_for_this_variable = False
    while True:
        tile = grid[y][x]
        if not tile.gender:
            return None

        if i_cant_find_name_for_this_variable:
            current_dir = get_next_direction_to_move(tile, current_dir, color)

        if current_dir == MALE and x == w - 1:
            return current_dir
        elif current_dir == FEMALE and x == 0:
            return current_dir
        elif current_dir == ASEXUAL and y == 0:
            return current_dir
        elif current_dir == POLYGENDER and y == h - 1:
            return current_dir
        
        if current_dir == MALE:
            x += 1
        elif current_dir == FEMALE:
            x -= 1
        elif current_dir == ASEXUAL:
            y -= 1
        elif current_dir == POLYGENDER:
            y += 1
        
        i_cant_find_name_for_this_variable = True

def check_for_straight_wins(grid):
    height = len(grid)
    width = len(grid[0])

    for i in range(height):
        tile = grid[i][0]
        if not tile.gender:
            continue

        x = 0
        y = i
        start_dir = None
        if tile.gender.pronouns == HE_HIM:
            start_dir = MALE
        elif tile.gender.pronouns == SHE_HER:
            start_dir = ASEXUAL
        elif tile.gender.pronouns == THEY_THEM:
            start_dir = POLYGENDER

        color = int(start_dir in tile.gender.femininity)
        
        if start_dir is not None:
            wall_dir = get_wall_hit(grid, start_dir, color, x, y, width, height)
            if wall_dir == MALE:
                return True

    for i in range(width):
        tile = grid[0][i]
        if not tile.gender:
            continue

        x = i
        y = 0
        start_dir = None

        if tile.gender.pronouns == HE_HIM:
            start_dir = POLYGENDER
        elif tile.gender.pronouns == SHE_HER:
            start_dir = FEMALE
        elif tile.gender.pronouns == THEY_THEM:
            start_dir = MALE

        color = int(start_dir in tile.gender.femininity)
        
        if start_dir is not None:
            wall_dir = get_wall_hit(grid, start_dir, color, x, y, width, height)
            if wall_dir == POLYGENDER:
                return True

    return False

def check_gay(grid, w, h, x, y, color):
    tile = grid[y][x]
    if not tile.gender:
        return False
    if tile.gay_checks[color]:
        return False

    dirs = None
    if color == TRANS:
        dirs = tile.gender.femininity
    else:
        dirs = tile.gender.masculinity
    
    def trans(_grid, _x, _y, _startdir):
        _tile = _grid[_y][_x]
        if not _tile.gender:
            return False

        if _tile.gay_checks[color]:
            return False
        
        _dir = _startdir
        i_cant_find_name_for_this_variable = False
        while not _tile.gay_checks[color]:
            _tile.gay_checks[color] = True

            if i_cant_find_name_for_this_variable:
                _dir = get_next_direction_to_move(_tile, _dir, color)
            if _dir == MALE and _x == w - 1:
                return False
            elif _dir == FEMALE and _x == 0:
                return False
            elif _dir == ASEXUAL and _y == 0:
                return False
            elif _dir == POLYGENDER and _y == h - 1:
                return False
            
            if _dir == MALE:
                _x += 1
            elif _dir == FEMALE:
                _x -= 1
            elif _dir == ASEXUAL:
                _y -= 1
            elif _dir == POLYGENDER:
                _y += 1
            
            i_cant_find_name_for_this_variable = True
            _tile = _grid[_y][_x]
            if not _tile.gender:
                return False

        return True
    
    if trans(grid, x, y, dirs[0]):
        return True
    tile.gay_checks[color] = False
    return trans(grid, x, y, dirs[1])

def check_for_gay_wins(grid):
    height = len(grid)
    width = len(grid[0])

    for y in range(height):
        for x in range(width):
            if check_gay(grid, width, height, x, y, TRANS) or check_gay(grid, width, height, x, y, CIS):
                return True
    return False

def parse(input_grid):
    input_grid = input_grid.split('\n')
    height = len(input_grid)
    width = max([len(line) for line in input_grid])

    for y in range(height):
        input_grid[y] += (width - len(input_grid[y])) * ' '

    grid = []
    for y in range(height):
        grid.append([])
        for x in range(width):
            grid[-1].append(Tile(grid))
            grid[-1][-1].make_possible_genders(input_grid, x, y)

    for y in range(height):
        for x in range(width):
            if len(grid[y][x].possible_genders) != 0:
                grid[y][x].collapse(True)
    return grid

def are_ya_winnin_son(board):
    board = parse(board)
    return check_for_straight_wins(board) or check_for_gay_wins(board)

