#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
using namespace std;

int width=1000,height=800;
sf::Vector2i M;
int X=0,Y=0;
int SNX,SNY;
enum Yon{
	Left,
	Right,
	Up,
	Down
};
int artma=50;
Yon y;
bool haraket=true;
int kuyruklar = 2;
int TAX[300];
int TAY[300];
int FRX,FRY;
bool die=false;
bool onTail=true;

int skor = 0;

template <typename T>
string toString(T arg)
{
    stringstream ss;
    ss << arg;
    return ss.str();
}



int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Turkish");
	sf::RenderWindow window(sf::VideoMode(width,height),"Snake");
	window.setFramerateLimit(60);
	sf::RectangleShape Wall1(sf::Vector2f(width,50));
	Wall1.setFillColor(sf::Color(0,255,255));
	sf::RectangleShape Wall2(sf::Vector2f(50,height));
	Wall2.setFillColor(sf::Color(0,255,255));
	sf::RectangleShape Snake(sf::Vector2f(50,50));
	Snake.setFillColor(sf::Color(0,255,0));
	sf::RectangleShape Fruit(sf::Vector2f(50,50));
	sf::RectangleShape Tail[300];
	
	sf::Font f;
	if(!f.loadFromFile("Font.ttf"))
		return EXIT_FAILURE;
	sf::Text Score("Skorun: "+toString(skor),f);
	sf::Text OB("Oyun Bitti!",f);
	OB.setColor(sf::Color(0,0,0));
	Score.setColor(sf::Color::Black);
	
	Snake.setOutlineThickness(1);
	Snake.setOutlineColor(sf::Color::Black);
	
	sf::Image icon;
	if(!icon.loadFromFile("icon.png"))return EXIT_FAILURE;
	
	window.setIcon(128,128,icon.getPixelsPtr());

	SNX=X+(width/2)-50;
	SNY=Y+(height/2)-50;
	for(int i=0;i<kuyruklar;i++)
	{
		if(i==0)
			TAX[i]=SNX+50;
		else
			TAX[i]=TAX[i-1]+50;
		TAY[i]=SNY;
		Tail[i].setSize(sf::Vector2f(50,50));
		
		Tail[i].setPosition(TAX[i],TAY[i]);
		Tail[i].setFillColor(sf::Color(255,0,0));
		Tail[i].setOutlineThickness(1);
		Tail[i].setOutlineColor(sf::Color::Black);
	}
	
	do
	{
		FRX=1+rand()%18;
		FRY=1+rand()%14;
		FRX*=50;
		FRY*=50;
		for(int i=0;i<kuyruklar;i++)
		{
			if(TAX[i] == FRX && TAY[i] == FRY)
				continue;
		}
		break;
	}while(true);
	
	sf::Clock c;
	sf::Time t;
	
	

	while(window.isOpen())
	{
		
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
				window.close();
		}
		M=sf::Mouse::getPosition(window);
if(!die)
{

		t=c.getElapsedTime();
		
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && y!=Right)
		{
			y=Left;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && y!=Left)
		{
			y=Right;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && y!=Up)
		{
			y=Down;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && y!=Down)
		{
			y=Up;
		}
		

		Fruit.setFillColor(sf::Color(255,0,128));
			
		if(!haraket)
		{
			if(operator>(t,sf::seconds(0.1)))
				haraket=true;
			
		}
		if(SNX == FRX && SNY == FRY)
		{
			skor+=10;
			Score.setString("Skorun: "+toString(skor));
			kuyruklar++;
			do
			{
				FRX=1+rand()%18;
				FRY=1+rand()%14;
				FRX*=50;
				FRY*=50;
				onTail=false;
				for(int i=0;i<kuyruklar;i++)
				{
					if(TAX[i] == FRX && TAY[i] == FRY)
						onTail=true;
				}
				if(!onTail)
					break;
			}while(true);
			
			for(int i=(kuyruklar-1);i>=0;i--)
			{
				Tail[i].setFillColor(sf::Color(255,0,0));
				Tail[i].setSize(sf::Vector2f(50,50));
				
			}
			TAX[kuyruklar-1]=TAX[kuyruklar-2];
			TAY[kuyruklar-1]=TAY[kuyruklar-2];
			Tail[kuyruklar-1].setPosition(TAX[kuyruklar-1],TAY[kuyruklar-1]);
			Tail[kuyruklar-1].setOutlineThickness(1);
			Tail[kuyruklar-1].setOutlineColor(sf::Color::Black);
			
		}
		if(haraket)
		{
			for(int i=(kuyruklar-1);i>=0;i--)
			{
				if(i==0)
				{
					TAX[i]=SNX;
					TAY[i]=SNY;
				}
					
				else
				{
					TAX[i]=TAX[i-1];
					TAY[i]=TAY[i-1];
				}
					
				Tail[i].setPosition(TAX[i],TAY[i]);
				Tail[i].setOutlineThickness(1);
				Tail[i].setOutlineColor(sf::Color::Black);
			}
		}
		if(y == Left && haraket)
			X-=artma;
		if(y == Right && haraket)
			X+=artma;
		if(y == Up && haraket)
			Y-=artma;
		if(y == Down && haraket)
			Y+=artma;
		SNX=X+(width/2)-50;
		SNY=Y+(height/2)-50;
		if(SNX<50)
		{
			die=true;
			continue;
		}	
		else if(SNX>width-100)
		{
			die=true;continue;
		}
		else if(SNY<50)
		{
			die=true;continue;
		}
		else if(SNY>height-100)
		{
			die=true;continue;
		}
		for(int i=0;i<kuyruklar;i++)
		{
			if(SNX == TAX[i] && SNY == TAY[i])
				die=true;continue;
		}
		
		Fruit.setPosition(FRX,FRY);
		Snake.setPosition(SNX,SNY);
		if(haraket)
		{
			c.restart();
			haraket=false;
			
		}
		
		window.clear();
		
		for(int i=0;i<kuyruklar;i++)
			window.draw(Tail[i]);

		Wall1.setPosition(0,0);
		Wall2.setPosition(0,0);
		window.draw(Wall1);
		window.draw(Wall2);
		Wall1.setPosition(0,height-50);
		Wall2.setPosition(width-50,0);
		window.draw(Wall1);
		window.draw(Wall2);
		window.draw(Snake);
		window.draw(Score);
		
		window.draw(Fruit);
		
		window.display();
}
if(die)
{
	Score.setPosition(width/2-95,height/2-100);
	OB.setPosition(width/2-85,height/2-150);
	OB.setString("Oyun Bitti!");
	window.clear(sf::Color(0,128,128));
	
	window.draw(Score);
	window.draw(OB);
	OB.setString("Yunus Emre Aydýn Tarafýndan Yapýlmýþtýr.");
	OB.setPosition(width/2-300,height/2-50);
	window.draw(OB);
	OB.setString("yunusemreaydin.ml");
	OB.setPosition(width/2-150,height/2);
	window.draw(OB);
	
	window.display();
	
}
	}
	
	
	

	
	return 0;
}
