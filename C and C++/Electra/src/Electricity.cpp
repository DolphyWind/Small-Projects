#include <Electricity.hpp>

Position makePosition(unsigned int x, unsigned int y)
{
    Position p;
    p.x = x;
    p.y = y;
    return p;
}

Electricity::Electricity(Position position, Direction direction) 
{
    m_position = position;
    m_direction = direction;
}
