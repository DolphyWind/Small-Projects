#include <Electra.hpp>

Electra::Electra(std::string filename) 
{
    readSourceCode(filename);
    createFirstOnes();
    step();
}

void Electra::step() 
{
    while (m_electricity.size() != 0)
    {
        std::vector<Electricity> new_ones;
        std::vector<std::size_t> gonna_remove;
        for(std::size_t i = 0; i < m_electricity.size(); i++)
        {
            move(m_electricity[i]);
            if(m_electricity[i].m_position.x < 0 || m_electricity[i].m_position.y < 0)
            {
                gonna_remove.push_back(i);
                continue;
            }
            if(m_electricity[i].m_position.x >= width || m_electricity[i].m_position.y >= height)
            {
                gonna_remove.push_back(i);
                continue;
            }
            
            auto c = m_code[ m_electricity[i].m_position.y ][ m_electricity[i].m_position.x ];

            switch (c)
            {
            case ' ':
                gonna_remove.push_back(i);
            break;
            case '-':
                {
                    if(!isHorizontal(m_electricity[i]))
                        gonna_remove.push_back(i);
                }
            break;
            case '|':
                {
                    if(!isVertical(m_electricity[i]))
                        gonna_remove.push_back(i);
                }
            break;
            case '~':
                {
                    if(isVertical(m_electricity[i]))
                        m_code[m_electricity[i].m_position.y][m_electricity[i].m_position.x] = '-';
                    else
                    {
                        if(m_electricity[i].m_direction == Direction::RIGHT) m_electricity[i].m_position.x --;
                        else if(m_electricity[i].m_direction == Direction::LEFT) m_electricity[i].m_position.x ++;
                    }
                }
            break;
            case '\\':
                {
                    if(isHorizontal(m_electricity[i]))
                        m_code[m_electricity[i].m_position.y][m_electricity[i].m_position.x] = '|';
                    else
                    {
                        if(m_electricity[i].m_direction == Direction::UP) m_electricity[i].m_position.x --;
                        else if(m_electricity[i].m_direction == Direction::DOWN) m_electricity[i].m_position.x ++;
                    }
                }
            break;
            case '*':
                {
                    if(!m_vector.empty())
                        std::cout << m_vector[m_vector.size() - 1] << std::endl;
                    fourWayMove(m_electricity[i], &new_ones);
                }
            break;
            case 'H':
                {
                    if(!m_vector.empty())
                        std::cout << (char)m_vector[m_vector.size() - 1] << std::flush;
                    fourWayMove(m_electricity[i], &new_ones);
                }
            break;
            case '+':
                {
                    fourWayMove(m_electricity[i], &new_ones);
                }
            break;
            case 'I':
                {
                    if(!isVertical(m_electricity[i]))
                        gonna_remove.push_back(i);
                    else
                        m_currentValue ++;
                }
            break;
            case '=':
                {
                    if(!isHorizontal(m_electricity[i]))
                        gonna_remove.push_back(i);
                    else
                        m_currentValue --;
                }
            break;
            case '!':
                {
                    if(!isVertical(m_electricity[i]))
                        gonna_remove.push_back(i);
                    else
                        m_vector.push_back(m_currentValue);
                }
            break;
            case 'L':
                {
                    if(m_electricity[i].m_direction == Direction::DOWN)
                    {
                        m_electricity[i].m_direction = Direction::RIGHT;
                        if(!m_vector.empty())
                            m_vector.pop_back();
                    }
                    else if(m_electricity[i].m_direction == Direction::LEFT)
                    {
                        m_electricity[i].m_direction = Direction::UP;
                        if(!m_vector.empty())
                            m_vector.pop_back();
                    }
                    else 
                        gonna_remove.push_back(i);
                }
            break;
            case 'M':
                {
                    if(m_vector.size() > 1)
                    {
                        m_vector[m_vector.size() - 2] *= m_vector[m_vector.size() - 1];
                        m_vector.pop_back();
                    }
                    fourWayMove(m_electricity[i], &new_ones);
                }
            break;
            case 'A':
                {
                    if(!isVertical(m_electricity[i]))
                        gonna_remove.push_back(i);
                    else
                    {
                        if(m_vector.size() > 1)
                        {
                            m_vector[m_vector.size() - 2] += m_vector[m_vector.size() - 1];
                            m_vector.pop_back();
                        }
                    }
                }
            break;
            case 'S':
                {
                    if(!isHorizontal(m_electricity[i]))
                        gonna_remove.push_back(i);
                    else
                    {
                        if(m_vector.size() > 1)
                        {
                            m_vector[m_vector.size() - 2] -= m_vector[m_vector.size() - 1];
                            m_vector.pop_back();
                        }
                    }
                }
            break;
            case '/':
                {
                    if(m_vector.size() > 1)
                    {
                        m_vector[m_vector.size() - 2] /= m_vector[m_vector.size() - 1];
                        m_vector.pop_back();
                    }
                    fourWayMove(m_electricity[i], &new_ones);
                }
            break;
            case '%':
                {
                    std::cin >> m_currentValue;
                    fourWayMove(m_electricity[i], &new_ones);
                }
            break;
            case '$':
                {
                    if(!isVertical(m_electricity[i]))
                        gonna_remove.push_back(i);
                    else if(!m_vector.empty())
                    {
                        double temp = m_currentValue;
                        m_currentValue = m_vector[m_vector.size() - 1];
                        m_vector[m_vector.size() - 1] = temp;
                    }
                }
            break;
            case '[':
                {
                    if(m_currentValue == 0)
                    {
                        if(isVertical(m_electricity[i]))
                        {
                            m_electricity[i].m_direction = Direction::LEFT;
                            new_ones.push_back(Electricity(m_electricity[i].m_position, Direction::RIGHT));
                        }
                    }
                    else
                    {
                        if(isHorizontal(m_electricity[i]))
                        {
                            m_electricity[i].m_direction = Direction::UP;
                            new_ones.push_back(Electricity(m_electricity[i].m_position, Direction::DOWN));
                        }
                    }
                }
            break;
            case ']':
                {
                    if(m_electricity.empty())
                    {
                        if(isHorizontal(m_electricity[i]))
                        {
                            m_electricity[i].m_direction = Direction::UP;
                            new_ones.push_back(Electricity(m_electricity[i].m_position, Direction::DOWN));
                        }
                    }
                    else if(m_currentValue == m_vector[m_vector.size() - 1])
                    {
                        if(isVertical(m_electricity[i]))
                        {
                            m_electricity[i].m_direction = Direction::LEFT;
                            new_ones.push_back(Electricity(m_electricity[i].m_position, Direction::RIGHT));
                        }
                    }
                    else
                    {
                        if(isHorizontal(m_electricity[i]))
                        {
                            m_electricity[i].m_direction = Direction::UP;
                            new_ones.push_back(Electricity(m_electricity[i].m_position, Direction::DOWN));
                        }
                    }
                }
            break;
            case '>':
            case '<':
                {
                    if(!isHorizontal(m_electricity[i]))
                        gonna_remove.push_back(i);
                }
            break;
            case '^':
            case 'v':
                {
                    if(!isVertical(m_electricity[i]))
                        gonna_remove.push_back(i);
                }
            break;
            case '#':
                {
                    m_currentValue = 0;
                    fourWayMove(m_electricity[i], &new_ones);
                }
            break;
            }
        }
        for(int i = gonna_remove.size() - 1; i >= 0; i--)
        {
            m_electricity.erase(m_electricity.begin() + gonna_remove[i]);
        }
        for(std::size_t i = 0; i < new_ones.size(); i++)
        {
            auto &e = new_ones[i];
            m_electricity.push_back(
                Electricity(e.m_position, e.m_direction)
            );
        }
        
        gonna_remove.clear();
        new_ones.clear();
    }
}

void Electra::createFirstOnes() 
{
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            char c = m_code[y][x];
            Position p;
            p.x = x;
            p.y = y;
            switch (c)
            {
            case '>':
                m_electricity.push_back(Electricity(p, Direction::RIGHT));
                break;
            case '^':
                m_electricity.push_back(Electricity(p, Direction::UP));
                break;
            case '<':
                m_electricity.push_back(Electricity(p, Direction::LEFT));
                break;
            case 'v':
                m_electricity.push_back(Electricity(p, Direction::DOWN));
                break;
            case 'x':
                {
                    m_electricity.push_back(Electricity(p, Direction::RIGHT));
                    m_electricity.push_back(Electricity(p, Direction::UP));
                    m_electricity.push_back(Electricity(p, Direction::LEFT));
                    m_electricity.push_back(Electricity(p, Direction::DOWN));
                }
                break;
            }
        }
    }
}

void Electra::move(Electricity &e) 
{
    switch (e.m_direction)
    {
    case Direction::RIGHT:
    e.m_position.x ++;
    break;
    case Direction::UP:
    e.m_position.y --;
    break;
    case Direction::LEFT:
    e.m_position.x --;
    break;
    case Direction::DOWN:
    e.m_position.y ++;
    break;
    }
}

std::vector<std::string> Electra::split(const std::string& str, const std::string& delim) 
{
    std::vector<std::string> tokens;
    std::size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if(pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

void Electra::readSourceCode(const std::string &filename) 
{
    std::ifstream file(filename);
    if(file.good())
    {
        std::string fileData;
        std::stringstream ss;
        ss << file.rdbuf();
        fileData = ss.str();

        if(fileData.find('\t') != std::string::npos) 
        {
            std::cout << "ERROR: Source code contains tab!" << std::endl;
            file.close();
            std::exit(1);
        }

        std::vector lines = split(fileData, "\n");
        height = lines.size();
        width = 0;
        for(auto &s : lines)
        {
            if(s.length() > width) width = s.length();
        }
        
        m_code = std::vector<std::vector<char>>(lines.size(), std::vector<char>(width, ' '));

        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < lines[i].length(); j++)
            {
                m_code[i][j] = lines[i][j];
            }
        }

    }
    else
    {
        std::cout << "Can\'t find " << filename << std::endl;
        std::exit(1);
    }
    file.close();
}

bool Electra::isHorizontal(Electricity &e) 
{
    if(e.m_direction == Direction::RIGHT || e.m_direction == Direction::LEFT)
        return true;
    return false;
}

bool Electra::isVertical(Electricity &e) 
{
    if(e.m_direction == Direction::DOWN || e.m_direction == Direction::UP)
        return true;
    return false;
}

void Electra::fourWayMove(Electricity &e, std::vector<Electricity> *v) 
{
    if(isHorizontal(e))
    {
        v->push_back(Electricity(e.m_position, Direction::UP));
        v->push_back(Electricity(e.m_position, Direction::DOWN));
        //std::cout << "H" << std::endl;
    }
    else if(isVertical(e))
    {
        v->push_back(Electricity(e.m_position, Direction::RIGHT));
        v->push_back(Electricity(e.m_position, Direction::LEFT));
        //std::cout << "V" << std::endl;
    }
}

void Electra::printVector() 
{
    for(int y = 0; y < m_code.size(); y++)
    {
        for(int x = 0; x < m_code[y].size(); x++)
        {
            std::cout << m_code[y][x];
        }
        std::cout << std::endl;
    }
}
