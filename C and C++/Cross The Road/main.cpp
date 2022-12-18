#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include "game.hpp"
using namespace std;

template <typename T>
string toString(T arg)
{
	stringstream ss;
	ss<<arg;
	return ss.str();
}
int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1000,800),"Cross The Road",sf::Style::Close);
	window.setFramerateLimit(60);
	sf::RectangleShape Road(sf::Vector2f(1000,480));
	Road.setFillColor(sf::Color(169,169,169));
	Road.setPosition(0,180);
	sf::RectangleShape Lines(sf::Vector2f(100,30));
	Lines.setPosition(100,490);
	const int CARS = 5000;
	const float PI = 3.14159265;
	Car car[CARS];
	int random = 0;
	int RD = 0;
	int score = 0;
	float speed = 0;
	float PlayerX = 475;
	float PlayerY = 700;
	float PSpeed = 4;
	bool finish = false;
	bool any = false;
	bool ilk = true;
	sf::CircleShape Player(25);
	Player.setFillColor(sf::Color(255,227,159));
	Player.setPosition(PlayerX+25,PlayerY+25);
	Player.setOrigin(25,25);
	sf::RectangleShape Line(sf::Vector2f(1,25));
	Line.setFillColor(sf::Color::Black);
	Line.setPosition(PlayerX+25,PlayerY+25);
	float rotation = -90;
	sf::Font f;
	if(!f.loadFromFile("Font.ttf"))
	{
		MessageBox(NULL, "\"Font.ttf\" is Missing!","ERROR",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	sf::Text t("",f);
	t.setColor(sf::Color(0,0,0));
	sf::Text Final("Game is Ended\n\n\n\tScore: "+toString(score),f);
	int number = 0;
	
	for(int i=0;i<CARS;i++)
	{
		random = rand()%3;
		RD = rand()%3;
		if(random == 0)
		{
			car[i].setSize(sf::Vector2f(120,120));
			car[i].setColor(sf::Color(11,102,35));		
		}		
		else if(random == 1)
		{
			car[i].setSize(sf::Vector2f(280,120));
			car[i].setColor(sf::Color(255,165,0));
		}
		else if(random == 2)
		{
			car[i].setSize(sf::Vector2f(420,120));
			car[i].setColor(sf::Color(255,69,0));
		}
		car[i].setPosition((ROAD)RD);
		car[i].setSpeed(10);
	}
	Player.rotate(1);
	sf::Clock cm;
	sf::Time tm;
	sf::Transform trans;
	
	while(window.isOpen())
	{
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
				window.close();
		}
if(!finish)
{
		tm = cm.getElapsedTime();
		if(tm>sf::seconds(0.5))
		{
			cm.restart();
			for(int i=0;i<CARS;i++)
			{
				if(!car[i].getMove())
				{
					car[i].setMove(true);
					break;
				}
			}
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			rotation +=7.5;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			rotation -=7.5;
		}
		Player.setRotation(-rotation);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			PlayerX += cos(Player.getRotation()*PI/180)*PSpeed;
			PlayerY += sin(Player.getRotation()*PI/180)*-PSpeed;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			PlayerX += cos(Player.getRotation()*PI/180)*-PSpeed;
			PlayerY += sin(Player.getRotation()*PI/180)*+PSpeed;
		}
		if(PlayerX+25>975)PlayerX = 950;
		if(PlayerX+25<25)PlayerX = 0;
		if(PlayerY+25>775)PlayerY = 750;
		if(PlayerY+25<25)
		{
			PlayerX = 500;
			PlayerY = 725;
			score++;
		}
		for(int i=0;i<CARS;i++)
		{
			if(PlayerX<=car[i].getPosition().x+car[i].getSize().x && PlayerX>=car[i].getPosition().x)
			{
				if(PlayerY<=car[i].getPosition().y+car[i].getSize().y && PlayerY>=car[i].getPosition().y)
					finish = true;
				if(PlayerY+50<=car[i].getPosition().y+car[i].getSize().y && PlayerY+50>=car[i].getPosition().y)
					finish = true;
			}
			if(PlayerX+50>=car[i].getPosition().x && PlayerX+50<=car[i].getPosition().x+car[i].getSize().x)
			{
				if(PlayerY+50<=car[i].getPosition().y+car[i].getSize().y && PlayerY+50>=car[i].getPosition().y)
					finish = true;
				if(PlayerY<=car[i].getPosition().y+car[i].getSize().y && PlayerY>=car[i].getPosition().y)
					finish = true;
			}
		}
		Player.setPosition(PlayerX+25,PlayerY+25);
		
		Line.setPosition(PlayerX+25,PlayerY+25);
		
		
		window.clear(sf::Color(0,255,0));
		
		window.draw(Road);
		for(int i=0;i<5;i++)
		{
			Lines.setPosition((i*200)+50,490);
			window.draw(Lines);
		}
		for(int i=0;i<5;i++)
		{
			Lines.setPosition((i*200)+50,310);
			window.draw(Lines);
		}
		for(int i=0;i<CARS;i++)
		{
			car[i].move();
			car[i].draw(window);
		}
		window.draw(Player);
		for(float i = -90+rotation;i<=90+rotation;i+=0.1)
		{
			Line.setRotation(i+90);
			window.draw(Line);
		}
		Line.setRotation(Player.getRotation());
		
		t.setString("Score: "+toString(score));
		window.draw(t);
		
		window.display();
}
else
{
	if(ilk)trans.rotate(2.5,500,500);
	ilk = false;
	Final.setPosition(375,300);
	Final.setString("Game is Ended\n\n\n\t  Score: "+toString(score));
	if(!any)
		trans.rotate(-0.5,500,500);
	else
		trans.rotate(0.5,500,500);
	number++;
	if(number%10 == 0)
	{
		any = !any;
		number = 0;
	}
		
	window.clear();
	
	window.draw(Final,trans);
	
	window.display();
}
	}
	
	
	
	return 0;
}
