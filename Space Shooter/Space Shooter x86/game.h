#ifndef game_h
#define game_h
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <Windows.h>
using namespace std;
#define BTYES 150
#define BTNO 151
#define BTCANCEL 152
#define BTOK 153

extern "C++"
{
	
	//For Convert Any Type To String
	template <typename T>
	string toString(T arg)
	{
		stringstream ss;
		ss<<arg;
		return ss.str();
	}
	//All Button Types
	enum Buttons
	{
		YESNO,
		OKCANCEL,
		ONLYOK,
		YESNOCANCEL,
		NOBUTTON
	};
	
	//Bullet Class
	class Bullet
	{
	public:
		sf::Sprite bullet;
		Bullet(sf::Texture *texture,sf::Vector2f pos)
		{
			bullet.setTexture(*texture);
			bullet.setScale(0.5,0.5);
			bullet.setPosition(pos);
		}
	};
	
	//Player Class
	class Player
	{
	public:
		sf::Sprite ship;
		int HP;
		int HPMAX;
		vector<Bullet> bullets;
		
		Player(sf::Texture *texture)
		{
			HPMAX = 15;
			HP = HPMAX;
			ship.setTexture(*texture);
		}
		
	};
	
	//Enemy Class
	class Enemy
	{
	public:
		sf::Sprite ship;
		int HPMAX,HP;
		int BOOMX,BOOMY;
		bool killed;
		Enemy(sf::Texture *texture,sf::Vector2f pos)
		{
			ship.setTexture(*texture);
			ship.setPosition(pos);
			HPMAX=15;
			HP=HPMAX;
			BOOMX=0;BOOMY=0;
			killed = false;
		}
	};
	
	//Button Class
	class Button
	{
	public:
		sf::RectangleShape btn;
		sf::Text t;
		
		Button(sf::Vector2f size,string str,sf::Font &f)
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
	
	//MessageBox Class
	class MB
	{
	public:
		sf::RectangleShape mb;
		sf::Text txt;
		bool appear;
		sf::Font fnt;
		Buttons btns;//YESNO ONLYOK OKCANCEL YESNOCANCEL NOBUTTON
		
		//Buttons
		Button OK;
		Button YES;
		Button NO;
		Button CANCEL;
		
		//Sets all
		MB(string str,Buttons bt,sf::RenderWindow &w,sf::Font &f)
		{
			appear = false;
			btns = bt;
			fnt = f;
			txt.setCharacterSize(15);
			mb.setFillColor(sf::Color(0,0,0,150));
			mb.setOutlineThickness(5);
			mb.setOutlineColor(sf::Color(0,0,0));
			mb.setSize(sf::Vector2f(300,200));
			txt.setFont(f);	
			txt.setString(str);
			txt.setFillColor(sf::Color(255,255,255));
			if(txt.getGlobalBounds().width>=mb.getSize().x)mb.setSize(sf::Vector2f(mb.getSize().x+20,mb.getSize().y));
			if(txt.getGlobalBounds().height>=mb.getSize().y)mb.setSize(sf::Vector2f(mb.getSize().x,mb.getSize().y+20));
			mb.setPosition(w.getSize().x/2-mb.getSize().x/2,w.getSize().y/2-mb.getSize().y/2);
			txt.setPosition(mb.getPosition().x+(mb.getSize().x/2)-txt.getGlobalBounds().width/2,mb.getPosition().y+(mb.getSize().y/2)-txt.getGlobalBounds().height/2);
			OK.setAll(sf::Vector2f(75,30),"OK",f);
			YES.setAll(sf::Vector2f(75,30),"YES",f);
			NO.setAll(sf::Vector2f(75,30),"NO",f);
			CANCEL.setAll(sf::Vector2f(75,30),"CANCEL",f);
			OK.t.setCharacterSize(15);OK.btn.setFillColor(sf::Color(0,0,0,150));OK.btn.setOutlineColor(sf::Color(0,0,0));
			YES.t.setCharacterSize(15);YES.btn.setFillColor(sf::Color(0,0,0,150));YES.btn.setOutlineColor(sf::Color(0,0,0));
			NO.t.setCharacterSize(15);NO.btn.setFillColor(sf::Color(0,0,0,150));NO.btn.setOutlineColor(sf::Color(0,0,0));
			CANCEL.t.setCharacterSize(15);CANCEL.btn.setFillColor(sf::Color(0,0,0,150));CANCEL.btn.setOutlineColor(sf::Color(0,0,0));
		
			if(bt == YESNO)
			{
				NO.setPosition(sf::Vector2f(mb.getSize().x+mb.getPosition().x-85,mb.getSize().y+mb.getPosition().y-40));
				YES.setPosition(sf::Vector2f(mb.getSize().x+mb.getPosition().x-180,mb.getSize().y+mb.getPosition().y-40));				
			}
			if(bt == ONLYOK)
			{
				OK.setPosition(sf::Vector2f(mb.getSize().x+mb.getPosition().x-85,mb.getSize().y+mb.getPosition().y-40));			
			}
			if(bt == OKCANCEL)
			{
				OK.setPosition(sf::Vector2f(mb.getSize().x+mb.getPosition().x-180,mb.getSize().y+mb.getPosition().y-40));	
				CANCEL.setPosition(sf::Vector2f(mb.getSize().x+mb.getPosition().x-85,mb.getSize().y+mb.getPosition().y-40));
			}
			if(bt == YESNOCANCEL)
			{
				NO.setPosition(sf::Vector2f(mb.getSize().x+mb.getPosition().x-180,mb.getSize().y+mb.getPosition().y-40));
				YES.setPosition(sf::Vector2f(mb.getSize().x+mb.getPosition().x-275,mb.getSize().y+mb.getPosition().y-40));				
				CANCEL.setPosition(sf::Vector2f(mb.getSize().x+mb.getPosition().x-85,mb.getSize().y+mb.getPosition().y-40));
			}
		}
		
		//returns which button was clicked 
		int draw(sf::RenderWindow &w)
		{
			if(appear)
			{
				w.draw(mb);
				w.draw(txt);
				if(btns == YESNO)
				{
					YES.ChangeCur(w,sf::Color::Yellow,sf::Color::Yellow,sf::Color(0,0,0),sf::Color(255,255,255));
					NO.ChangeCur(w,sf::Color::Yellow,sf::Color::Yellow,sf::Color(0,0,0),sf::Color(255,255,255));
					NO.draw(w);
					YES.draw(w);
					if(YES.CanClickable(sf::Mouse::getPosition(w)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						return BTYES;
					if(NO.CanClickable(sf::Mouse::getPosition(w)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						return BTNO;
				}
				if(btns == ONLYOK)
				{
					OK.ChangeCur(w,sf::Color::Yellow,sf::Color::Yellow,sf::Color(0,0,0),sf::Color(255,255,255));
					OK.draw(w);
					if(OK.CanClickable(sf::Mouse::getPosition(w)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						return BTOK;			
				}
				if(btns == YESNOCANCEL)	
				{
					YES.ChangeCur(w,sf::Color::Yellow,sf::Color::Yellow,sf::Color(0,0,0),sf::Color(255,255,255));
					NO.ChangeCur(w,sf::Color::Yellow,sf::Color::Yellow,sf::Color(0,0,0),sf::Color(255,255,255));
					CANCEL.ChangeCur(w,sf::Color::Yellow,sf::Color::Yellow,sf::Color(0,0,0),sf::Color(255,255,255));
					NO.draw(w);
					YES.draw(w);
					CANCEL.draw(w);
					if(YES.CanClickable(sf::Mouse::getPosition(w)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						return BTYES;
					if(NO.CanClickable(sf::Mouse::getPosition(w)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						return BTNO;
					if(CANCEL.CanClickable(sf::Mouse::getPosition(w)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						return BTCANCEL;
				}
				if(btns == OKCANCEL)
				{
					OK.ChangeCur(w,sf::Color::Yellow,sf::Color::Yellow,sf::Color(0,0,0),sf::Color(255,255,255));
					CANCEL.ChangeCur(w,sf::Color::Yellow,sf::Color::Yellow,sf::Color(0,0,0),sf::Color(255,255,255));
					OK.draw(w);
					CANCEL.draw(w);
					if(OK.CanClickable(sf::Mouse::getPosition(w)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						return BTOK;
					if(CANCEL.CanClickable(sf::Mouse::getPosition(w)) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
						return BTCANCEL;
				}
			}	
			return 0;
		}
		
	};
}


#endif
