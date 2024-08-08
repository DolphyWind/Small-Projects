#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include "Headers/Buttons.h"
#include "Headers/game.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;

inline unsigned int getRandomValue(int min, int max)
{
	return min+rand()%(max-min);
}

enum BOOL3
{
	DOGRU,
	DOGRUYANLIS,
	YANLIS
};

template <typename T>
string toString(T arg)
{
	stringstream ss;
	ss << arg;
	return ss.str();
}

bool kontrol(vector<Wall> w,float x,float y)
{
	sf::RectangleShape shp(sf::Vector2f(25,25));	
	shp.setPosition(x,y);	
	for(int i=0;i<w.size();i++)
	{
		if(w[i].wall.getGlobalBounds().intersects(shp.getGlobalBounds()))
			return false;
	}
	if(x>=1000 || x<0 || y>=800 || y<0)
		return false;
	return true;
}

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Turkish");
	sf::RenderWindow window(sf::VideoMode(1000,800),"Labirent 2 - Yunus Emre Aydýn",sf::Style::Close);
	window.setFramerateLimit(30);
	sf::Image icon;
	if(!icon.loadFromFile("icons/icon.png"))
	{
		MessageBox(NULL,"Ýkon Dosyasý Açýlamadý Lütfen Oyun Dosyalarýnýzýn Tam Olduðundan Emin Olun","Hata",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
	
	sf::Font Bfont;
	if(!Bfont.loadFromFile("Fonts/ButtonFont.ttf"))
	{
		MessageBox(NULL, "\"Fonts/ButtonFont.ttf\" Açýlamadý!","Hata!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	sf::Font font;
	if(!font.loadFromFile("Fonts/Font.ttf"))
	{
		MessageBox(NULL, "\"Fonts/Font.ttf\" Açýlamadý!","Hata!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	int mainmenu = true;
	Button Play(sf::Vector2f(300,60),"Oyna",Bfont);
	Play.setPosition(sf::Vector2f(500-Play.btn.getGlobalBounds().width/2,450));
	Button Exit(sf::Vector2f(300,60),"Çýkýþ",Bfont);
	Exit.setPosition(sf::Vector2f(Play.btn.getPosition().x,660));
	Button Controls(sf::Vector2f(300,60),"Kontroller",Bfont);
	Controls.setPosition(sf::Vector2f(500-Play.btn.getGlobalBounds().width/2,520));
	Button Items(sf::Vector2f(300,60),"Bloklar",Bfont);
	Items.setPosition(sf::Vector2f(500-Items.btn.getGlobalBounds().width/2,590));
	
	bool click = true;
	sf::Clock ClickClock;
	sf::Time ClickTimer;
	
	sf::Text site("www.yunusemreaydin.ml",font);
	site.setCharacterSize(80);
	site.setFillColor(sf::Color(0,0,0,50));
	site.setOrigin(site.getGlobalBounds().width/2,site.getGlobalBounds().height/2);
	site.setPosition(500,400);
	site.setRotation(getRandomValue(0,360));
	
	string next;
	ifstream first("Levels/ilkbolumadi.txt");
	first >> next;
	
	string suanki = next;
	
	next = "Levels/"+next;
	
	ifstream bolum(next.c_str());
	
	int ws = 0;
	int rs = 0;
	int ffs = 0;
	bool bitti = false;
	bool move = true;
	
	bool lostFocus = false;
	
	bolum >> ws;
	
	vector<Wall> walls;
	vector<Teleporter> randoms;
	vector<FakeFinish> fakes;
	
	for(int i=0;i<ws;i++)
	{
		float x,y;
		bolum >> x;
		bolum >> y;
		walls.push_back(Wall(sf::Vector2f(x,y),sf::Vector2f(25,25)));
	}
	
	bolum >> rs;
	for(int i=0;i<rs;i++)
	{
		float x,y;
		bolum >> x;
		bolum >> y;
		randoms.push_back(Teleporter(x,y));
	}
	
	bolum >> ffs;
	for(int i = 0; i < ffs; i++)
	{
		float x,y;
		bolum >> x;
		bolum >> y;
		fakes.push_back(FakeFinish(sf::Vector2f(x,y),sf::Vector2f(25,25)));
	}
	float x,y;
	bolum >> x;
	bolum >> y;
	Player player(sf::Vector2f(x,y),sf::Vector2f(25,25));
	sf::Vector2f firstPos(sf::Vector2f(x,y));
	bolum >> x;
	bolum >> y;
	Finish finish(x,y,sf::Vector2f(25,25));
	next = "";
	bolum >> next;
	if(next == "FINAL")
		bitti = true;
	
	sf::Clock PressClock;
	sf::Time PressTimer;
	bool press = true;
	
	sf::Text suankiT(suanki,font);
	suankiT.setOrigin(0,suankiT.getGlobalBounds().height/2);
	sf::RectangleShape Lacivert(sf::Vector2f(suankiT.getGlobalBounds().width+20,suankiT.getGlobalBounds().height+50));
	BOOL3 lcvrt = DOGRU;
	sf::Clock lcvrtClock;
	sf::Time lcvrtTimer;
	Lacivert.setPosition(-Lacivert.getSize().x,100);
	Lacivert.setFillColor(sf::Color(0,0,205));
	
	while(window.isOpen())
	{
		sf::Event e;
		while(window.pollEvent(e))
		{
			switch(e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::LostFocus:
				lostFocus = true;
				break;
			case sf::Event::GainedFocus:
				lostFocus = false;
				break;
			}
		}
		PressTimer = PressClock.getElapsedTime();
		ClickTimer = ClickClock.getElapsedTime();
		if(lcvrt!=DOGRUYANLIS)lcvrtClock.restart();
		lcvrtTimer = lcvrtClock.getElapsedTime();
		
		
		if(!mainmenu)window.setTitle("Labirent 2 - Yunus Emre Aydýn - "+suanki);
		
		if(!click)if(ClickTimer>sf::seconds(0.5))click = true;
		
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !lostFocus)
		{
			if(click)
			{
				if(mainmenu)
				{
					if(Exit.CanClickable(sf::Mouse::getPosition(window)))
					{
						window.close();
					}
					if(Play.CanClickable(sf::Mouse::getPosition(window)))
					{
						mainmenu = false;
					}
					if(Controls.CanClickable(sf::Mouse::getPosition(window)))
					{
						MessageBox(NULL, "W / Yukarý Ok Tuþu:\tYukarý\n---------------------------------------------------------\nS / Aþaðý Ok Tuþu:\t\tAþaðý\n---------------------------------------------------------\nA / Sol Ok Tuþu:\t\tSol\n---------------------------------------------------------\nD / Sað Ok Tuþu:\t\tSað\n---------------------------------------------------------\nShift:\t\t\tYavaþ Git","Kontroller",MB_OK | MB_ICONINFORMATION);
					}
					if(Items.CanClickable(sf::Mouse::getPosition(window)))
					{
						MessageBox(NULL, "Oyuncu\n---------------------------------------------------------\nRenk: Yeþil.\nÖzekllik: Sizin Kontrolünüzde Olan Bloktur.\n---------------------------------------------------------\nBitiþ\n---------------------------------------------------------\nRenk: Pembe.\nÖzellik: Sizi Bir Sonraki Bölüme Geçiren Bloktur.\n---------------------------------------------------------\nDuvar\n---------------------------------------------------------\nRenk: Beyaz.\nÖzellikler: Sizin Arkasýna Geçmenize Engel Olan Bloktur.\n---------------------------------------------------------\nIþýnlayýcý\n---------------------------------------------------------\nRenk: Rengarenk.\nÖzellik: Sizi Rastgele Bir Yere Iþýnlayan Bloktur .\n---------------------------------------------------------\nSahte Bitiþ\n---------------------------------------------------------\nRenk: Pembe.\nÖzellik: Deðdiðinizde Sizi En Baþa Iþýnlayan ve Yok Olan Bloktur.\n---------------------------------------------------------\n","Blok Özellikleri",MB_OK | MB_ICONINFORMATION);
					}
					click = false;
				}
				
			}
		}
			
		
		if(!press)
		{
			if(PressTimer>sf::seconds(0.1))press = true;
		}
		
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && !mainmenu && kontrol(walls,player.ply.getPosition().x,player.ply.getPosition().y-25) && !lostFocus)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			{
				if(press)
				{
					player.ply.move(0,-25);
					PressClock.restart();press = false;
				}
			}
			else if(kontrol(walls,player.ply.getPosition().x,player.ply.getPosition().y-25))
				player.ply.move(0,-25);
				
		}
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && !mainmenu && kontrol(walls,player.ply.getPosition().x-25,player.ply.getPosition().y) && !lostFocus)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			{
				if(press)
				{
					player.ply.move(-25,0);
					PressClock.restart();press = false;
				}
			}
			else if(kontrol(walls,player.ply.getPosition().x-25,player.ply.getPosition().y))
				player.ply.move(-25,0);
		}
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && !mainmenu && kontrol(walls,player.ply.getPosition().x+25,player.ply.getPosition().y) && !lostFocus)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			{
				if(press)
				{
					player.ply.move(25,0);
					PressClock.restart();press = false;
				}
			}
			else if(kontrol(walls,player.ply.getPosition().x+25,player.ply.getPosition().y))
				player.ply.move(25,0);
		}
		
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && !mainmenu && kontrol(walls,player.ply.getPosition().x,player.ply.getPosition().y+25) && !lostFocus)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			{
				if(press)
				{
					player.ply.move(0,25);
					PressClock.restart();press = false;
				}
			}
			else if(kontrol(walls,player.ply.getPosition().x,player.ply.getPosition().y+25))
				player.ply.move(0,25);
		}
		
		if(press) PressClock.restart();
		
		player.Check(window);
		for(int i=0;i<randoms.size();i++)
		{
			if(randoms[i].random.getGlobalBounds().intersects(player.ply.getGlobalBounds()))
				player.ply.setPosition(randoms[i].generateRandomPos(window,finish,walls,randoms,fakes));
		}
		for(int i = 0; i < fakes.size(); i++)
		{
			if(fakes[i].FF.getGlobalBounds().intersects(player.ply.getGlobalBounds()))
			{
				fakes.erase(fakes.begin()+i);
				player.ply.setPosition(firstPos);
			}
				
		}
		if(finish.fin.getGlobalBounds().intersects(player.ply.getGlobalBounds()))
		{
			if(bitti)
			{
				window.close();
			}
			bolum.close();
			walls.clear();
			randoms.clear();
			fakes.clear();
			suanki = next;
			suankiT.setString(suanki);
			Lacivert.setSize(sf::Vector2f(suankiT.getGlobalBounds().width+20,suankiT.getGlobalBounds().height+50));
			move = true;
			lcvrt = DOGRU;
			string openfile = "Levels/" + next;
			bolum.open(openfile.c_str());
			bolum>>ws;
			for(int i=0;i<ws;i++)
			{
				float x,y;
				bolum >> x;
				bolum >> y;
				walls.push_back(Wall(sf::Vector2f(x,y),sf::Vector2f(25,25)));
			}
			bolum >> rs;
			for(int i=0;i<rs;i++)
			{
				float x,y;
				bolum >> x;
				bolum >> y;
				randoms.push_back(Teleporter(x,y));
			}
			bolum >> ffs;
			for(int i=0;i<ffs;i++)
			{
				float x,y;
				bolum >> x;
				bolum >> y;
				fakes.push_back(FakeFinish(sf::Vector2f(x,y),sf::Vector2f(25,25)));
			}
			float x,y;
			bolum >> x;
			bolum >> y;
			player.ply.setPosition(x,y);
			firstPos = sf::Vector2f(x,y);
			float Fx,Fy;
			bolum >> Fx;
			bolum >> Fy;
			finish.fin.setPosition(Fx,Fy);
			bolum >> next;
			if(next == "FINAL")
				bitti = true;
		}
		
		suankiT.setPosition(Lacivert.getPosition().x+(Lacivert.getSize().x/2-suankiT.getGlobalBounds().width/2),Lacivert.getPosition().y+(Lacivert.getSize().y/2-suankiT.getGlobalBounds().height/2));
		
		
		window.clear(sf::Color(mainmenu*255,mainmenu*255,mainmenu*255));
		
		if(mainmenu)
		{
			site.rotate(1);
			window.draw(site);
			Play.ChangeCur(window);
			Exit.ChangeCur(window);
			Controls.ChangeCur(window);
			Items.ChangeCur(window);
			Play.draw(window);
			Exit.draw(window);
			Controls.draw(window);
			Items.draw(window);
		}		
		else
		{
			player.draw(window);
			finish.draw(window);
			for(int i=0;i<walls.size();i++)
				walls[i].draw(window);
			for(int i=0;i<randoms.size();i++)
			{
				randoms[i].random.setFillColor(sf::Color(rand()%256,rand()%256,rand()%256));
				randoms[i].draw(window);
			}
			for(int i = 0; i < fakes.size(); i++)
				fakes[i].draw(window);
			if(move && !lostFocus)
			{
				if(Lacivert.getPosition().x<0 && lcvrt == DOGRU)
					Lacivert.move(2,0);
				else if(Lacivert.getPosition().x>=0)
				{
					lcvrt = DOGRUYANLIS;
					if(lcvrtTimer>sf::seconds(2))
					{
						lcvrt = YANLIS;
					}
				}
				if(lcvrt == YANLIS)
				{
					Lacivert.move(-2,0);
					if(Lacivert.getPosition().x+Lacivert.getSize().x<=0)
						move = false;
				}
			}
			window.draw(Lacivert);
			window.draw(suankiT);
		}
		
		window.display();
	}
	
	return 0;
}
