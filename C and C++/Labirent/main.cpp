#include <windows.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int bloksayisi;
int *BloklarX,*BloklarY;
char *file;
string s;
int PX,PY,FX,FY,BX,BY;
bool haraket=true;
bool bitti=false;

bool Kontrol(int x,int y,int arttirma,int dikeyartma)
{
	for(int i=0;i<bloksayisi;i++)
		if(BloklarX[i]==x+arttirma && BloklarY[i]==y+dikeyartma)
			return false;
	return true;
}
template<typename T>
string toString(T arg)
{
	stringstream ss;
	ss<<arg;
	return ss.str();
}
int main()
{
	sf::RenderWindow window(sf::VideoMode(1000,800),"Labirent Oyunu - Level1.lvl");
	window.setFramerateLimit(60);
	ifstream Dosya;


	sf::Clock c;
	sf::Time t;
	
	
	Dosya.open("Level1.lvl");
	Dosya>>bloksayisi;
	BloklarX = new int[bloksayisi];
	BloklarY = new int[bloksayisi];
	sf::RectangleShape Bloklar[bloksayisi];
	for(int i=0;i<bloksayisi;i++)
		Bloklar[i].setSize(sf::Vector2f(50,40));
	for(int i=0;i<bloksayisi;i++)
	{
		Dosya>>BX;
		Dosya>>BY;
		Bloklar[i].setPosition(BX,BY);
		BloklarX[i]=BX;
		BloklarY[i]=BY;
	}
	Dosya>>PX;
	Dosya>>PY;
	Dosya>>FX;
	Dosya>>FY;
	Dosya>>s;
	file = new char [s.length()+1];
	copy(s.begin(),s.end(),file);

	
	sf::RectangleShape Player(sf::Vector2f(50,40));
	Player.setPosition(PX,PY);
	sf::RectangleShape Fin(sf::Vector2f(50,40));
	Fin.setPosition(FX,FY);
	Player.setFillColor(sf::Color(0,128,0));
	Fin.setFillColor(sf::Color(255,0,255));
	
	while(window.isOpen())
	{
		t=c.getElapsedTime();
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
				window.close();
		}
		if(haraket)
			c.restart();
		if(!haraket && operator>(t,sf::seconds(0.1)))
			haraket=true;
			/*
		if(PX<0)
			PX=0;
		if(PX+50>1000)
			PX=1000;
		if(PY<0)
			PY=0;
		if(PY+40>800)
			PY=800;*/
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && Kontrol(PX,PY,50,0) && haraket)
		{
			PX+=50;
			haraket=false;
		}		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && Kontrol(PX,PY,-50,0) && haraket)
		{
			PX-=50;
			haraket=false;
		}		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && Kontrol(PX,PY,0,40) && haraket)
		{
			PY+=40;
			haraket=false;
		}		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && Kontrol(PX,PY,0,-40) && haraket)
		{
			PY-=40;
			haraket=false;
		}
		
		if(PX<0)
			PX=0;
		if(PX+50>1000)
			PX=950;
		if(PY<0)
			PY=0;
		if(PY+40>800)
			PY=760;	
		window.clear();
		Player.setPosition(PX,PY);
		Fin.setPosition(FX,FY);
		
		if(PX==FX && PY==FY)
		{
			haraket=false;
			c.restart();
			Dosya.close();
			if(bitti)
				window.close();
			Dosya.open(file);
			
			window.setTitle("Labirent Oyunu - "+toString(file));
			Dosya>>bloksayisi;
			BloklarX = new int[bloksayisi];
			BloklarY = new int[bloksayisi];
			for(int i=0;i<bloksayisi;i++)
			{
				Bloklar[i].setSize(sf::Vector2f(50,40));
				haraket=false;
				c.restart();
			}
			
			for(int i=0;i<bloksayisi;i++)
			{
				Dosya>>BX;
				Dosya>>BY;
				Bloklar[i].setPosition(BX,BY);
				BloklarX[i]=BX;
				BloklarY[i]=BY;
				haraket=false;
				c.restart();
			}
			Dosya>>PX;
			Dosya>>PY;
			Dosya>>FX;
			Dosya>>FY;
			Dosya>>s;
			if(s=="FINAL")
				bitti=true;
			file = new char [s.length()+1];
			copy(s.begin(),s.end(),file);
		}
			
		for(int i=0;i<bloksayisi;i++)
			window.draw(Bloklar[i]);
		window.draw(Player);
		window.draw(Fin);
		
		window.display();
	}
	
	
	
	

	Dosya.close();
	delete [] BloklarX;
	delete [] BloklarY;
	MessageBox(0, "-Yunus Emre Aydýn Tarafýndan Yapýlmýþtýr.\nOynadýðýnýz Ýçin Teþekkürler.\nyunusemreaydin.ml", "Son Söz", MB_OK);
	return 0;
}
