#ifndef game_hpp
#define game_hpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

extern "C++"
{
	enum ROAD
	{
		_1 = 0,
		_2 = 1,
		_3 = 2
	};
	class Car
	{
		private:
			sf::Vector2f Size;
			sf::Vector2f Position;
			float speed;
			bool m_bmove;
			sf::RectangleShape rect;
			ROAD road;
		public:
			bool usedOnce;
			Car()
			{
				speed = 5;
				m_bmove = false;
				usedOnce = false;
			}
			bool getMove()
			{
				return m_bmove;
			}
			void setMove(bool b)
			{
				m_bmove = b;
			}
			void setSpeed(float spd)
			{
				speed = spd;
			}
			void reGo()
			{
				int random = 0;
				int RD;
				random = rand()%3;
				RD = rand()%3;
				if(random == 0)
				{
					rect.setSize(sf::Vector2f(120,120));
					rect.setFillColor(sf::Color(11,102,35));			
				}		
				else if(random == 1)
				{
					rect.setSize(sf::Vector2f(280,120));
					rect.setFillColor(sf::Color(255,165,0));
				}
				else if(random == 2)
				{
					rect.setSize(sf::Vector2f(420,120));
					rect.setFillColor(sf::Color(255,69,0));
				}
				if(RD == _1)
				{
					rect.setPosition(-Size.x,185);
				}
				if(RD == _2)
				{
					rect.setPosition(-Size.x,350);
				}
				if(RD == _3)
				{
					rect.setPosition(-Size.x,530);
				}
				m_bmove = false;
			}
			void setSize(sf::Vector2f size)
			{
				Size = size;
				rect.setSize(Size);
			}
			ROAD getRoad()
			{
				return road;
			}
			void setPosition(ROAD r)
			{
				road = r;
			
				if(road == _1)
				{
					Position = sf::Vector2f(-Size.x,185);
				}
				if(road == _2)
				{
					Position = sf::Vector2f(-Size.x,350);
				}
				if(road == _3)
				{
					Position = sf::Vector2f(-Size.x,530);
				}		
				rect.setPosition(Position);
			}
			sf::Vector2f getSize()
			{
				return Size;
			}
			sf::Vector2f getPosition()
			{
				return Position;
			}
			void move()
			{	
				if(m_bmove)
				{
					Position.x+=speed;
					rect.setPosition(Position);
				}	
			}
			void draw(sf::RenderWindow &w)
			{
				w.draw(rect);
			}
			void setColor(sf::Color c)
			{
				rect.setFillColor(c);
			}
	};
	
	
	
}

#endif
