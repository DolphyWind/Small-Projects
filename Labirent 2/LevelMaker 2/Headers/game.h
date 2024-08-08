#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
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
		void Check(sf::RenderWindow &window)
		{
			if(ply.getPosition().x < 0)ply.setPosition(0,ply.getPosition().y);
			if(ply.getPosition().y < 0)ply.setPosition(ply.getPosition().x,0);
			if(ply.getPosition().x+ply.getSize().x>window.getSize().x)ply.setPosition(ply.getPosition().x,window.getSize().x-ply.getSize().x);
			if(ply.getPosition().y+ply.getSize().y>window.getSize().y)ply.setPosition(ply.getPosition().x,window.getSize().y-ply.getSize().y);
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
		void draw(sf::RenderWindow &window)
		{
			window.draw(wall);
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
		sf::RectangleShape randomize;
		Teleporter(sf::Vector2f pos)
		{
			randomize.setPosition(pos);
			randomize.setSize(sf::Vector2f(25,25));
			randomize.setFillColor(sf::Color(255,69,0));
		}
		void draw(sf::RenderWindow &window)
		{
			window.draw(randomize);
		}
	};
	class FakeFinish
	{
	public:
		sf::RectangleShape FF;
		FakeFinish(sf::Vector2f pos)
		{
			FF.setPosition(pos);
			FF.setSize(sf::Vector2f(25,25));
			FF.setFillColor(sf::Color(255,0,128));
		}
		void draw(sf::RenderWindow &window)
		{
			window.draw(FF);
		}
	};
}
