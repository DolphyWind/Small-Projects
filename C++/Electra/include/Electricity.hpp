#pragma once
#include <vector>
#define VVC std::vector<std::vector<char>>

struct Position
{
    unsigned int x;
    unsigned int y;
};
enum class Direction
{
    RIGHT = 0,
    UP,
    LEFT,
    DOWN
};

class Electricity
{
public:
    Position m_position;
    Direction m_direction;

    Electricity(Position position, Direction direction);
    ~Electricity() {};
};