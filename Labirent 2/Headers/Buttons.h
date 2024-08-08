#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <Windows.h>
using namespace std;

extern "C++"
{
	class Button
	{
	public:
		sf::RectangleShape btn;
		sf::Text t;
		
		Button(sf::Vector2f size,string str,sf::Font &f,unsigned int charsize = 20,sf::Color c = sf::Color(0,191,255))
		{
			btn.setSize(size);
			t.setFont(f);
			t.setString(str);
			if(size.x<t.getGlobalBounds().width)size.x = size.x+40;
			if(size.y<t.getGlobalBounds().height)size.y = size.y+40;
			btn.setOutlineThickness(5);
			btn.setOutlineColor(sf::Color::Red);
			btn.setFillColor(c);
			t.setColor(sf::Color::Red);
			t.setCharacterSize(charsize);
		}
		Button(){};
		void setPosition(sf::Vector2f position)
		{
			btn.setPosition(position);
			t.setPosition(btn.getPosition().x+(btn.getSize().x/2 - t.getGlobalBounds().width/2),btn.getPosition().y+(btn.getSize().y/2 - t.getGlobalBounds().height/2));
		}
		
		//It's constructure
		void setAll(sf::Vector2f size,string str,sf::Font &f)
		{
			btn.setSize(size);
			t.setFont(f);
			t.setString(str);
			if(size.x<t.getGlobalBounds().width)size.x = size.x+40;
			if(size.y<t.getGlobalBounds().height)size.y = size.y+40;
			btn.setOutlineThickness(5);
			btn.setOutlineColor(sf::Color::Red);
			btn.setFillColor(sf::Color(0,191,255));
			t.setColor(sf::Color::Red);
			t.setCharacterSize(20);
		}
		void draw(sf::RenderWindow &w)
		{
			w.draw(btn);
			w.draw(t);	
		}
		bool CanClickable(sf::Vector2i ms)
		{
			return (ms.x>=btn.getPosition().x && ms.y>=btn.getPosition().y && ms.x<=btn.getPosition().x+btn.getSize().x && ms.y<=btn.getPosition().y+btn.getSize().y);
		}
		
		//For Change Outline Color etc.
		void ChangeCur(sf::RenderWindow &w)
		{
			if(CanClickable(sf::Mouse::getPosition(w)))
			{
				SetCursor(LoadCursor(NULL,IDC_HAND));
				btn.setOutlineColor(sf::Color(138,43,226));
				t.setColor(sf::Color(138,43,226));
			}
			else
			{
				btn.setOutlineColor(sf::Color::Red);
				t.setColor(sf::Color::Red);
			}
		}
		void ChangeCur(sf::RenderWindow &w,sf::Color c1,sf::Color c2,sf::Color o1,sf::Color o2)
		{
			if(CanClickable(sf::Mouse::getPosition(w)))
			{
				SetCursor(LoadCursor(NULL,IDC_HAND));
				btn.setOutlineColor(c1);
				t.setColor(c2);
			}
			else
			{
				btn.setOutlineColor(o1);
				t.setColor(o2);
			}
		}
	};
}
