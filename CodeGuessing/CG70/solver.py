# solver?
# i just,, cant

import requests
import heapq
from math import fabs, sqrt

session_token = ""
with open('./session.txt', 'r') as f:
    session_token = f.read()

url = "https://codeguessing.gay/extra/game70"
headers = {"Content-Type": "application/json"}
cookies = {"session": session_token}

def distance(p1, p2):
    return fabs(p1[0] - p2[0]) + fabs(p1[1] - p2[1]) + sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)

def map_(func, l):
    return list(map(func, l))

def diff(path):
    diff_list = []
    for i in range(len(path) - 1):
        diff_list.append( (path[i + 1][0] - path[i][0], path[i + 1][1] - path[i][1]) )
    return diff_list

def direction(d):
    match d:
        case (-1, 0):
            return "left"
        case (1, 0):
            return "right"
        case (0, -1):
            return "up"
        case (0, 1):
            return "down"
    raise RuntimeError("le boobs")


def solve():
    while True:
        r = requests.get(url, cookies=cookies)
        if r.status_code != 200:
            raise RuntimeError(f"server issue {r.status_code}")

        resp = r.json()
        
        grid = resp['grid']
        s = resp['s']
        score = resp['score']
        print(f"score: {score}")
        print(f"s: {s}")
        print(grid, end='\n\n')
        grid = grid.split('\n')
        ylimit = len(grid)
        xlimit = len(grid[0])

        x = 0
        y = 0
        player = (0, 0)
        bomb = (-1, -1)
        target = (0, 0)
        for y, line in enumerate(grid):
            for x, c in enumerate(line):
                if c == '@':
                    player = (x, y)
                elif c == '+':
                    target = (x, y)
                elif c == '*':
                    bomb = (x, y)

        # i cant be bothered to implement a good pathfinding algorithm
        # diff = [target[0] - player[0], target[1] - player[1]]
        # vert_movement = 0
        # hrzt_movement = 0
        #
        # if diff[0] < 0:
        #     hrzt_movement = 1
        #     diff[0] = -diff[0]
        # if diff[1] > 0:
        #     vert_movement = 1
        # else:
        #     diff[1] = -diff[1]
        #
        # dirs = []
        # dirs += ['rliegfhtt'[hrzt_movement::2]] * diff[0] # cool trick
        # dirs += [('down' if vert_movement == 1 else 'up')] * diff[1] # not so cool trick

        # the code above keeps hitting bombs
        # this is intentional by lyly's server code
        # literally 1984
        pq = []
        d = distance(player, target)
        heapq.heappush(pq, (d, player))

        path = []
        while True:
            if len(pq) == 0:
                break
            _, (x, y) = heapq.heappop(pq)
            path.append((x, y))
            if x == target[0] and y == target[1]:
                break

            for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                _x = x + dx
                _y = y + dy
                if (_x == bomb[0] and _y == bomb[1]) or not (_x >= 0 and _x < xlimit) or not (_y >= 0 and _y < ylimit):
                    continue

                d = distance((_x, _y), target)
                heapq.heappush(pq, (d, (_x, _y)))

        path = diff(path)
        dirs = map_(direction, path)

        for dir in dirs:
            r = requests.post(url, json={'dir': dir}, headers=headers, cookies=cookies)

        if score >= 2024:
            break

solve()
