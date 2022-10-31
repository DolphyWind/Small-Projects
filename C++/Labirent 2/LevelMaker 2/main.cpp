#include <SFML/Graphics.hpp>
#include <iostream>
#include "Headers/game.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
using namespace std;

template <typename T>
string toString(T arg)
{
	stringstream ss;
	ss<<arg;
	return ss.str();
}

enum Sira
{
	Duvar = 0,
	Oyuncu = 1,
	Bitis = 2,
	Rastgele = 3,
	FF = 4
};

string SiraToString(Sira s)
{
	if(s == Duvar)
		return "Duvar";
	else if(s == Oyuncu)
		return "Oyuncu";
	else if(s == Bitis)
		return "Bitiþ";
	else if(s == Rastgele)
		return "Teleporter";
	else if(s == FF)
		return "Fake Finish";
}

inline bool kontrol(float x,float y,float pX, float pY)
{
	return (x == pX && y == pY);
}
inline string BoolToString(bool b)
{
	return b?"true":"false";
}
bool KNT(sf::RectangleShape shp, vector<Wall> w)
{
	for(int i=0;i<w.size();i++)
	{
		if(shp.getGlobalBounds().intersects(w[i].wall.getGlobalBounds()))
			return false;
	}
	return true;
}
bool KNT(sf::RectangleShape shp,vector<Teleporter> r)
{
	for(int i=0;i<r.size();i++)
	{
		if(shp.getGlobalBounds().intersects(r[i].randomize.getGlobalBounds()))
			return false;
	}
	return true;
}
bool KNT(sf::RectangleShape shp,vector<FakeFinish> r)
{
	for(int i=0;i<r.size();i++)
	{
		if(shp.getGlobalBounds().intersects(r[i].FF.getGlobalBounds()))
			return false;
	}
	return true;
}

int main()
{
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos( consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	setlocale(LC_ALL, "Turkish");
	
	sf::RenderWindow window(sf::VideoMode(1000,800),"Levelmaker 2 - Yunus Emre Aydýn",sf::Style::Close);
	window.setFramerateLimit(60);
	sf::Image icon;
	if(!icon.loadFromFile("icons/icon.png"))
	{
		MessageBox(NULL,"Ýkon Dosyasý Açýlamadý Lütfen Oyun Dosyalarýnýzýn Tam Olduðundan Emin Olun","Hata",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
	
	bool text = true;
	sf::Font font;
	sf::Font TextFont;
	if(!font.loadFromFile("Fonts/Font.ttf"))
	{
		MessageBox(NULL,"\"Fonts/Font.ttf\" Açýlamadý!","Hata",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	if(!TextFont.loadFromFile("Fonts/TextFont.ttf"))
	{
		MessageBox(NULL,"\"Fonts/TextFont.ttf\" Açýlamadý!","Hata",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	sf::Text txt("Kayýt Dosyasýnýn Adýný Giriniz",font);
	txt.setPosition(500-txt.getGlobalBounds().width/2,400-txt.getGlobalBounds().height);
	sf::Text filename("",font);
	
	ofstream yaz;
	string str;
	string DStr;
	
	sf::RectangleShape koyucu(sf::Vector2f(25,25));
	koyucu.setPosition(sf::Vector2f(0,0));
	koyucu.setFillColor(sf::Color::Blue);
	
	vector<Wall> walls;
	Sira sira = Duvar; 
	bool lostFocus = false;
	vector<FakeFinish> fakes;
	Player player(sf::Vector2f(25,25));
	Finish finish(sf::Vector2f(25,25));
	vector<Teleporter> randoms;
	
	int wheel = 0;
	
	bool t1 = true,t2 = false;
	
	sf::Text after("",font);
	
	while(window.isOpen())
	{
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
			{
				if((player.placed && finish.placed) || text)
					window.close();
				else
					MessageBox(NULL,"Çýkmak Ýçin Oyuncuyu Ve Bitiþi Koymanýz Gerekir!","Hata",MB_OK | MB_ICONERROR);
			}
			if(e.type == sf::Event::TextEntered && t1 && !lostFocus)
			{
				if(e.text.unicode < 128)
				{
					if(e.text.unicode == 8)
					{
						if(str.size()>0)
							str.resize(str.size()-1);
					}				
					else if(e.text.unicode == 13)
					{
						t1 = false;
						t2 = true;	
						txt.setString("Bundan Sonraki Levelin Adý(Eðer Bu Son Bölüm Ýse FINAL yazýnýz)");
						txt.setPosition(500-txt.getGlobalBounds().width/2,400-txt.getGlobalBounds().height);
						yaz.open(str.c_str());
						str = "";	
					}			
					else
						str += static_cast<char>(e.text.unicode);
				}
				filename.setString(str);
				filename.setPosition(500-filename.getGlobalBounds().width/2,400+filename.getGlobalBounds().height);
			}
			else if(e.type == sf::Event::TextEntered && t2 && !lostFocus)
			{
				if(e.text.unicode < 128)
				{
					if(e.text.unicode == 8)
					{
						if(str.size()>0)
							str.resize(str.size()-1);
					}				
					else if(e.text.unicode == 13)
					{
						t2 = false;
						text = false;
					}		
					else
						str += static_cast<char>(e.text.unicode);
				}
				after.setString(str);
				after.setPosition(500-after.getGlobalBounds().width/2,400+after.getGlobalBounds().height);
			}
			if(e.type == sf::Event::LostFocus)lostFocus = true;
			if(e.type == sf::Event::GainedFocus)lostFocus = false;
			if(!text && e.type == sf::Event::MouseWheelMoved)
			{
				wheel += e.mouseWheel.delta;
				if(wheel < 0) wheel = 4;
				if(wheel > 4) wheel = 0;
				sira = (Sira)wheel;
			}
		}	
	
		koyucu.setPosition(sf::Mouse::getPosition(window).x-sf::Mouse::getPosition(window).x%25,sf::Mouse::getPosition(window).y-sf::Mouse::getPosition(window).y%25);
		if(koyucu.getPosition().x<0)koyucu.setPosition(0,koyucu.getPosition().y);
		if(koyucu.getPosition().y<0)koyucu.setPosition(koyucu.getPosition().x,0);
		if(koyucu.getPosition().x+25>1000)koyucu.setPosition(975,koyucu.getPosition().y);
		if(koyucu.getPosition().y+25>800)koyucu.setPosition(koyucu.getPosition().x,775);
		
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !text && !lostFocus)
		{
			if(sira == Oyuncu)
			{
				player.ply.setPosition(koyucu.getPosition());
				player.placed = true;
			}
			else if(sira == Bitis)
			{
				finish.fin.setPosition(koyucu.getPosition());
				finish.placed = true;
			}
			else if(sira == Duvar)
			{
				if(KNT(koyucu,walls))
					walls.push_back(Wall(koyucu.getPosition(),sf::Vector2f(25,25)));
			}
			else if(sira == Rastgele)
			{
				if(KNT(koyucu,randoms))
					randoms.push_back(Teleporter(koyucu.getPosition()));
			}
			else if(sira == FF)
			{
				if(KNT(koyucu,fakes))
					fakes.push_back(FakeFinish(koyucu.getPosition()));
			}
		}
		if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && !lostFocus && !text)
		{
			if(koyucu.getGlobalBounds().intersects(player.ply.getGlobalBounds()))
			{
				player.placed = false;
			}
			else if(koyucu.getGlobalBounds().intersects(finish.fin.getGlobalBounds()))
			{
				finish.placed = false;
			}
			else
			{
				for(int i = 0;i < walls.size();i++)
				{
					if(walls[i].wall.getGlobalBounds().intersects(koyucu.getGlobalBounds()))
						walls.erase(walls.begin()+i);
				}
				for(int i=0;i<randoms.size();i++)
				{
					if(randoms[i].randomize.getGlobalBounds().intersects(koyucu.getGlobalBounds()))
						randoms.erase(randoms.begin()+i);
				}
				for(int i = 0; i < fakes.size(); i++)
				{
					if(fakes[i].FF.getGlobalBounds().intersects(koyucu.getGlobalBounds()))
						fakes.erase(fakes.begin()+i);
				}
			}
		}
		
		
		window.clear();
		
		if(text)
		{
			window.draw(txt);
			if(t1)
				window.draw(filename);
			if(t2)
				window.draw(after);
		}
		else
		{
			if(player.placed)player.draw(window);
			if(finish.placed)finish.draw(window);
			for(int i=0;i<walls.size();i++)
				window.draw(walls[i].wall);
			for(int i=0;i<randoms.size();i++)
				randoms[i].draw(window);
			for(int i = 0; i < fakes.size(); i++)
				fakes[i].draw(window);
			window.draw(koyucu);
		}
		
		window.display();
		system("cls");
		cout<<"Koyucu\'nun X Koordinati: "<<toString(koyucu.getPosition().x)<<endl
			<<"Koyucu\'nun Y Koordinati: "<<toString(koyucu.getPosition().y)<<endl
			<<"Oyuncu Koyuldu Mu?: "<<BoolToString(player.placed)<<endl
			<<"Bitiþ Koyuldu Mu?: "<<BoolToString(finish.placed)<<endl
			<<"Þu Anki Koyulacak Nesne: "<<SiraToString(sira)<<endl
			<<"Koyulan Duvarlar: "<<walls.size()<<endl
			<<"Koyulan Teleporterlar: "<<randoms.size()<<endl
			<<"Koyulan FakeFinishler: "<<fakes.size()<<endl;
	}
	
	if(!text)
	{
		yaz<<walls.size()<<endl<<endl;
		for(int i=0;i<walls.size();i++)
			yaz << walls[i].wall.getPosition().x << " " << walls[i].wall.getPosition().y << endl;
		yaz << endl << endl;
		yaz << randoms.size()<<endl<<endl;
		for(int i=0;i<randoms.size();i++)
			yaz << randoms[i].randomize.getPosition().x << " " << randoms[i].randomize.getPosition().y << endl;
		yaz << endl << endl;
		yaz << fakes.size() << endl << endl;
		for(int i = 0; i < fakes.size(); i++)
			yaz << fakes[i].FF.getPosition().x << " " << fakes[i].FF.getPosition().y << endl;
		yaz << player.ply.getPosition().x << " " << player.ply.getPosition().y << endl << endl;	
		yaz << finish.fin.getPosition().x << " " << finish.fin.getPosition().y << endl << endl;
		yaz << str;
		
		MessageBox(NULL,"Bölümünüz Baþarýyla Oluþturuldu!","Baþarýlý",MB_OK);
	}
	yaz.close();
	
	return 0;
}
