from typing import Tuple
from dataclasses import dataclass
from enum import Enum

class Direction(Enum):
    RIGHT = 0
    UP = 1
    LEFT = 2
    DOWN = 3

@dataclass
class BoxInfo:
    x: int
    y: int
    width: int
    height: int

@dataclass
class ConnectionInfo:
    x: int
    y: int
    direction: Direction
    box_index: int

VALID_CHARACTERS = '─│┌┐└┘├┤┬┴ \n'

def get_box_info(input: list[str], x: int, y: int) -> BoxInfo | None | bool:
    if input[y][x] != '┌':
        return None

    width = 1
    height = 1
    try:
        while input[y][x + width] in ('─', '┴'):
            width += 1
    except:
        pass
    try:
        while input[y + height][x] in ('│', '┤'):
            height += 1
    except:
        pass
    try:
        if input[y + height][x] in ('├', '┬', '┤') or input[y][x + width] in ('├', '┬', '┤') or input[y + height][x + width] in ('├', '┬', '┤'):
            return False
        if input[y + height][x] != '└' or input[y][x + width] != '┐':
            return None
    except:
        return None

    for i in range(width):
        if input[y + height][x + i] not in ('└', '─', '┬', '┘'):
            return False
    
    for i in range(height):
        if input[y + i][x + width] not in ('┐', '│', '├', '┘'):
            return False
    return BoxInfo(x, y, width + 1, height + 1)

def find_connections(input: list[str], box: BoxInfo, box_index) -> list[ConnectionInfo]:
    x, y = box.x, box.y
    w, h = box.width, box.height

    result = []

    for i in range(w):
        if input[y][x + i] == '┴':
            result.append(ConnectionInfo(x + i, y, Direction.UP, box_index))
        if input[y + h - 1][x + i] == '┬':
            result.append(ConnectionInfo(x + i, y + h - 1, Direction.DOWN, box_index))

    for i in range(h):
        if input[y + i][x] == '┤':
            result.append(ConnectionInfo(x, y + i, Direction.LEFT, box_index))
        if input[y + i][x + w - 1] == '├':
            result.append(ConnectionInfo(x + w - 1, y + i, Direction.RIGHT, box_index))

    return result

def char_to_directions(c) -> set[Direction] | None:
    match c:
        case '─':
            return {Direction.LEFT, Direction.RIGHT}
        case '│':
            return {Direction.DOWN, Direction.UP}
        case '┌':
            return {Direction.DOWN, Direction.RIGHT}
        case '┐':
            return {Direction.DOWN, Direction.LEFT}
        case '└':
            return {Direction.UP, Direction.RIGHT}
        case '┘':
            return {Direction.UP, Direction.LEFT}
        case '├':
            return {Direction.RIGHT}
        case '┤':
            return {Direction.LEFT}
        case '┬':
            return {Direction.DOWN}
        case '┴':
            return {Direction.UP}
    return None

def direction_to_delta_pos(d: Direction) -> Tuple[int, int]:
    match d:
        case Direction.LEFT:
            return (-1, 0)
        case Direction.RIGHT:
            return (1, 0)
        case Direction.UP:
            return (0, -1)
        case Direction.DOWN:
            return (0, 1)

def invert(d: Direction) -> Direction:
    i = d.value
    return Direction((i + 2) % 4)

def follow_connection(input: list[str], conn: ConnectionInfo) -> ConnectionInfo | None:
    LINES = '─│┌┐└┘├┤┬┴'
    pointer_x = conn.x
    pointer_y = conn.y
    pointer_d = conn.direction
    
    while True:
        dx, dy = direction_to_delta_pos(pointer_d)
        pointer_x += dx
        pointer_y += dy
        c = input[pointer_y][pointer_x]
        
        directions = char_to_directions(c)
        if directions is None:
            return None
        if len(directions) == 1:
            return ConnectionInfo(pointer_x, pointer_y, list(directions)[0], -1)
        
        if invert(pointer_d) not in directions:
            return None
        
        directions.remove(invert(pointer_d))
        pointer_d = list(directions)[0]


def entry(input: str):
    if any(char not in VALID_CHARACTERS for char in input):
        return "nahhh"

    input_as_list: list[str] = input.split('\n')
    boxes: list[BoxInfo] = []

    # Find boxes
    for y, line in enumerate(input_as_list):
        index = -1
        while (index := line.find('┌', index + 1)) != -1:
            box_info = get_box_info(input_as_list, index, y)
            if box_info is None:
                continue
            if box_info == False:
                return "nahhh"
            boxes.append(box_info)

    for i in range(len(boxes)):
        for j in range(len(boxes)):
            if i == j:
                continue
            b1 = boxes[i]
            b2 = boxes[j]

            if b1.x < b2.x and b2.x < b1.x + b1.width and b1.y < b2.y and b2.y < b1.y + b1.height:
                return "nahhh"

    # Find connections
    connections: list[ConnectionInfo] = []
    for i, box in enumerate(boxes):
        connections.extend(find_connections(input_as_list, box, i))
    
    formed_connections = []
    while len(connections) != 0:
        first_connection = connections[0]
        end_connection = follow_connection(input_as_list, first_connection)
        if end_connection is None:
            return "nahhh"
        
        for c in connections:
            if c.x == end_connection.x and c.y == end_connection.y and c.direction == end_connection.direction:
                end_connection.box_index = c.box_index
                break
        if end_connection.box_index == -1:
            return "nahhh"
        
        if end_connection.box_index == connections[0].box_index:
            return "nahhh"

        if (connections[0].box_index, end_connection.box_index) in formed_connections or (end_connection.box_index, connections[0].box_index) in formed_connections:
            return "nahhh"

        formed_connections.append((connections[0].box_index, end_connection.box_index))

        connections.remove(end_connection)
        del connections[0]

    return "legit frfr"

