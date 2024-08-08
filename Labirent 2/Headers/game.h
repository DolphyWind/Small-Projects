#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;

extern "C++"
{
	class Player
	{
	public:
		sf::RectangleShape ply;
		bool placed;
		
		Player(sf::Vector2f pos,sf::Vector2f size)
		{
			ply.setPosition(pos);
			ply.setFillColor(sf::Color(0,128,0));
			ply.setSize(size);
			placed = false;
		}
		Player(float X, float Y,sf::Vector2f size)
		{
			ply.setPosition(X,Y);
			ply.setFillColor(sf::Color(0,128,0));
			ply.setSize(size);
			placed = false;
		}
		Player(sf::Vector2f size)
		{
			ply.setFillColor(sf::Color(0,128,0));
			ply.setSize(size);
			placed = false;
		}
		void Check(sf::RenderWindow &w)
		{
			if(ply.getPosition().x<0)
				ply.move(25,0);
			if(ply.getPosition().y<0)
				ply.move(0,25);
			if(ply.getPosition().x+ply.getSize().x>w.getSize().x)
				ply.move(-25,0);
			if(ply.getPosition().y+ply.getSize().y>w.getSize().y)
				ply.move(0,-25);
		}
		void draw(sf::RenderWindow &window)
		{
			window.draw(ply);
		}
		void move(float mX, float mY)
		{
			ply.move(mX,mY);
		}

		
	};
	class Wall
	{
	public:
		sf::RectangleShape wall;
		Wall(sf::Vector2f pos,sf::Vector2f size)
		{
			wall.setSize(size);
			wall.setPosition(pos);
			wall.setFillColor(sf::Color::White);
		}
		Wall(sf::Vector2f size)
		{
			wall.setSize(size);
			wall.setFillColor(sf::Color::White);
		}
		Wall()
		{
			wall.setSize(sf::Vector2f(25,25));
			wall.setFillColor(sf::Color::White);
		}
		void draw(sf::RenderWindow &window)
		{
			window.draw(wall);
		}
	};
	class FakeFinish
	{
	public:
		sf::RectangleShape FF;
		FakeFinish(sf::Vector2f pos,sf::Vector2f size)
		{
			FF.setSize(size);
			FF.setPosition(pos);
			FF.setFillColor(sf::Color(255,0,128));
		}
		void draw(sf::RenderWindow &w)
		{
			w.draw(FF);
		}
	};
	class Finish
	{
	public:
		bool placed;
		sf::RectangleShape fin;
		
		Finish(sf::Vector2f size)
		{
			fin.setSize(size);
			fin.setFillColor(sf::Color(255,0,128));
			placed = false;
		}
		Finish(sf::Vector2f pos,sf::Vector2f size)
		{
			fin.setSize(size);
			fin.setFillColor(sf::Color(255,0,128));
			fin.setPosition(pos);
			placed = false;
		}
		Finish(float X,float Y,sf::Vector2f size)
		{
			fin.setSize(size);
			fin.setFillColor(sf::Color(255,0,128));
			fin.setPosition(X,Y);
			placed = false;
		}
		void draw(sf::RenderWindow &window)
		{
			window.draw(fin);
		}
	};
	class Teleporter
	{
	public:
		sf::RectangleShape random;
		
		Teleporter(float x, float y)
		{
			random.setSize(sf::Vector2f(25,25));
			random.setPosition(x,y);
		}
		sf::Vector2f generateRandomPos(sf::RenderWindow &w,Finish &f,vector<Wall> &wall,vector<Teleporter> &r,vector<FakeFinish> &fake)
		{
			sf::RectangleShape shp(sf::Vector2f(25,25));
			bool boolean = false;
			do
			{
				boolean = false;
				shp.setPosition(rand()%(w.getSize().x-25),rand()%(w.getSize().y-25));
				shp.setPosition(shp.getPosition().x-((int)shp.getPosition().x%25),shp.getPosition().y-((int)shp.getPosition().y%25));
				for(int i=0;i<wall.size();i++)
				{
					if(wall[i].wall.getGlobalBounds().intersects(shp.getGlobalBounds()))
						boolean = true;
				}
				if(f.fin.getGlobalBounds().intersects(shp.getGlobalBounds()))
					boolean = true;
				for(int i=0;i<r.size();i++)
				{
					if(r[i].random.getGlobalBounds().intersects(shp.getGlobalBounds()))
						boolean = true;
				}
				for(int i=0;i<fake.size();i++)
				{
					if(fake[i].FF.getGlobalBounds().intersects(shp.getGlobalBounds()))
						boolean = true;
				}
			}while(boolean);
			return shp.getPosition();
		}
		void ChangeColor()
		{
			random.setFillColor(sf::Color(rand()%256,rand()%256,rand()%256));
		}
		void draw(sf::RenderWindow &w)
		{
			w.draw(random);
		}
	};
	
}
