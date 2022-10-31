#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

bool yazildi=false;
sf::Vector2i Mouse;
string filename;
int KX,KY;
int KarelerX[398],KarelerY[398];
int PX,PY;
int FX,FY;
int sayac=0;
bool haraket=true;
bool PlayerPlaced=false;
bool FinishPlaced=false;
enum Mode{
	Wall,
	Play,
	Final
};
Mode m;

template<typename T>
string toString(T arg)
{
	stringstream ss;
	ss<<arg;
	return ss.str();
}

int main()
{
	setlocale(LC_ALL, "Turkish");
	sf::RenderWindow window(sf::VideoMode(1000,800),"Bölüm Yapýcý");
	window.setFramerateLimit(60);
	
	ofstream Dosya;
	char *file;
	
	sf::Font f;
	if(!f.loadFromFile("Font.ttf"))
		return EXIT_FAILURE;
	
	sf::Text fn("Dosya Adýný Giriniz: ",f);
	sf::Text MousePos("",f);
	MousePos.setPosition(0,500);
	sf::RectangleShape KareS(sf::Vector2f(50,40));
	KareS.setFillColor(sf::Color(128,128,128));
	sf::RectangleShape Kareler[398];
	sf::RectangleShape Player(sf::Vector2f(50,40));
	sf::RectangleShape Bitis(sf::Vector2f(50,40));
	Bitis.setFillColor(sf::Color(255,0,255));
	Player.setFillColor(sf::Color(0,128,0));
	
	for(int i=0;i<398;i++)
	{
		Kareler[i].setSize(sf::Vector2f(50,40));
	}
	sf::Clock c;
	sf::Time t;
	while(window.isOpen())
	{
		t=c.getElapsedTime();
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
				window.close();
			if(e.type == sf::Event::TextEntered && !yazildi)
			{
				if(e.text.unicode<128)
				{
					if(e.text.unicode == 8)
					{
						if(filename.size()>0)
							filename.resize(filename.size()-1);
					}						
					else if(e.text.unicode == 13)
					{
						yazildi=true;
						file = new char[filename.length()+1];
						copy(filename.begin(),filename.end(),file);
						Dosya.open(file);
						if(!Dosya.is_open())break;
						window.setTitle("Bölüm Yapýcý - "+toString(file));
					}
					else
						filename += static_cast<char>(e.text.unicode);
					fn.setString("Dosya Adýný Giriniz: "+filename);
					
				}
				
			}
		}
		Mouse = sf::Mouse::getPosition(window);
		if(haraket)
			c.restart();
		if(!haraket && operator>(t,sf::seconds(0.1)))
			haraket=true;
		
		if(yazildi)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && haraket)
			{
				KX+=50;
				haraket=false;		
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && haraket)
			{
				KX-=50;
				haraket=false;
			}				
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && haraket)
			{
				KY-=40;
				haraket=false;
			}				
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && haraket)
			{
				KY+=40;
				haraket=false;
			}
			if(KX<0)
				KX=0;
			if(KX+50>1000)
				KX=950;
			if(KY<0)
				KY=0;
			if(KY+40>800)
				KY=760;	
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && haraket)
			{
				if(m == Wall)
				{
					KarelerX[sayac]=KX;
					KarelerY[sayac]=KY;
					Kareler[sayac].setPosition(KX,KY);
					sayac++;
				}
				else if(m == Play)
				{
					PX=KX;
					PY=KY;
					PlayerPlaced=true;
				}
				else if(m == Final)
				{
					FX=KX;
					FY=KY;
					FinishPlaced=true;
				}
				haraket=false;
				
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			{
				sayac=0;
				PlayerPlaced=false;
				FinishPlaced=false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && haraket)
			{
				m=Play;
			
				haraket=false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && haraket)
			{
				m=Final;
				haraket=false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && haraket)
			{
				m=Wall;
				haraket=false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && haraket)
			{
				if(KX==PX && KY==PY)
					PlayerPlaced=false;
				if(KX==FX && KY==FY)
					FinishPlaced=false;
				else
					sayac--;
				haraket=false;
				if(sayac<0)
					sayac=0;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && PlayerPlaced && FinishPlaced)
			{
				window.close();
			}
			
			KareS.setPosition(KX,KY);
			Player.setPosition(PX,PY);
			Bitis.setPosition(FX,FY);
			
		}
		
		MousePos.setString(toString(Mouse.x)+" "+toString(Mouse.y));
		
		window.clear();	
		
		if(!yazildi)
		{
			
			window.draw(fn);
			
		}
	
			
		for(int i=0;i<sayac;i++)
			window.draw(Kareler[i]);
		if(PlayerPlaced)
			window.draw(Player);
		if(FinishPlaced)
			window.draw(Bitis);
		if(yazildi)
			window.draw(KareS);
		window.display();
	}
	Dosya<<sayac<<endl;
	for(int i=0;i<sayac;i++)
		Dosya<<KarelerX[i]<<" "<<KarelerY[i]<<endl;
	Dosya<<endl<<endl<<PX<<" "<<PY<<endl<<endl<<FX<<" "<<FY;

	Dosya.close();
	delete [] file;
	
	return 0;
}
