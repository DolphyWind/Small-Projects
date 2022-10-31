#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <sstream>
using namespace std;

template<typename T>
string toString(T arg)
{
	stringstream ss;
	ss<<arg;
	return ss.str();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(600,600),"Saat",sf::Style::Close);
	sf::RenderWindow window1(sf::VideoMode(600,200),"Saat",sf::Style::Close);	
	window.setFramerateLimit(60);
	window1.setFramerateLimit(60);
	
	sf::VideoMode Desktop = sf::VideoMode::getDesktopMode();
	Desktop.width++;
	Desktop.height++;
	
	window.setPosition(sf::Vector2i(Desktop.width/2-600,Desktop.height/2-300));
	window1.setPosition(sf::Vector2i(Desktop.width/2+10,Desktop.height/2-100));
	
	int X=0;
	
	
	sf::Image icon;
	
	if(!icon.loadFromFile("icon.png"))
		return EXIT_FAILURE;
	sf::Vector2u iconSize;
	iconSize = icon.getSize();
	
	window.setIcon(iconSize.x,iconSize.y,icon.getPixelsPtr());
	window1.setIcon(iconSize.x,iconSize.y,icon.getPixelsPtr());
	
	SYSTEMTIME time;
	

	float YelkovanRad=180;
	float AkrepRad=180;
	float SaniyeRad=180;
	float Saniyeekle = 0;
	float Hour,Second,Minute,MiliSecond;
	float DakikaEkle = 0;
	
	sf::Color col;
	col.r = 128;
	col.g = 128;
	col.b = 128;
	col.a = 200;
	
	sf::Image ShadowI[100];
	for(int i=0;i<100;i++)
	{
		if(i>=5)
			col.a-=2;
		
		ShadowI[i].create(1,290,col);	
	}
	
	col.a = 200;
	sf::Texture ShadowT[100];
	for(int i=0;i<100;i++)
		ShadowT[i].loadFromImage(ShadowI[i]);
	sf::Sprite Shadow[100];
	for(int i=0;i<100;i++)
	{
		Shadow[i].setTexture(ShadowT[i]);
		Shadow[i].setPosition(300,300);
	}
	
	
	sf::Texture Saat;
	if(!Saat.loadFromFile("Saat.png"))
		return EXIT_FAILURE;
	sf::Sprite SaatS(Saat);
	
	sf::Font f;
	if(!f.loadFromFile("Font.ttf"))
		return 0;
	sf::Text text("",f);
	text.setColor(sf::Color::White);
	text.setCharacterSize(150);
	
	sf::RectangleShape Yelkovan(sf::Vector2f(2,290));
	Yelkovan.setFillColor(sf::Color::Black);
	Yelkovan.setPosition(300,300);
	
	sf::Clock cl;
	sf::Time tm;
	
	sf::RectangleShape Akrep(sf::Vector2f(4,150));
	Akrep.setFillColor(sf::Color::Black);
	Akrep.setPosition(300,300);
	
	sf::RectangleShape Saniye(sf::Vector2f(1,290));
	Saniye.setFillColor(sf::Color::Black);
	Saniye.setPosition(300,300);
	
	string timestr;
	
	while(window.isOpen() || window1.isOpen())
	{
		GetLocalTime(&time);
		Hour = time.wHour;
		Minute = time.wMinute;
		Second = time.wSecond;
		MiliSecond = time.wMilliseconds;
		tm = cl.getElapsedTime();
		Saniyeekle = MiliSecond/(1000/6);
		DakikaEkle = Second/10;
		
		timestr = "";
		if(Hour<10)
			timestr+="0";
		timestr += toString(Hour)+":";
		if(Minute<10)
			timestr+="0";
		timestr += toString(Minute)+":";
		if(Second<10)
			timestr+="0";
		timestr += toString(Second);
		
		text.setString(timestr);
			
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
			{
				window.close();
				window1.close();
			}
				
		}
		while(window1.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
			{
				window.close();
				window1.close();
			}
		}
		
		YelkovanRad = Minute*6+180+DakikaEkle;
		Yelkovan.setRotation(YelkovanRad);
		
		AkrepRad = Minute/2+(Hour*30+180);
		Akrep.setRotation(AkrepRad);
		
		SaniyeRad = Second*6+180+Saniyeekle-1;
		Saniye.setRotation(SaniyeRad);
		
		window.clear(sf::Color::White);
		
		window.draw(SaatS);
		window.draw(Yelkovan);
		window.draw(Akrep);
		window.draw(Saniye);
		for(float i=0;i<10;i+=0.1)
		{
			X=i*10;
			Shadow[X].setRotation(SaniyeRad-i);
			//window.draw(Shadow[X]);
			
		}
		window.display();
		window1.clear();
		
		window1.draw(text);
		
		
		window1.display();
	}
	
	
	return 0;
}
