#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale.h>
#include "Button.h"
#include "TextBox.h"
using namespace std;
using namespace Form;

int main()
{
	setlocale(LC_ALL, "Turkish");
	sf::RenderWindow window(sf::VideoMode(1000,800),"Yarýþ Oyunu",sf::Style::Close);
	window.setFramerateLimit(60);
	window.setPosition(sf::Vector2i(window.getPosition().x,25));
	int area = 0;
	sf::Font f;
	if(!f.loadFromFile("Font.ttf"))
	{
		MessageBox(NULL, "Bir Dosya Açýlamadý", "Hata", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	TextBox O1(window,f,150,"Oyuncu 1\'in Adý",15);
	O1.setPosition(250,150);
	TextBox O2(window,f,150,"Oyuncu 2\'nin Adý",15);
	O2.setPosition(600,150);
	Button Kaydet(sf::Vector2f(300,60), f, "Baþla!", 15);
	string O1N,O2N;
	Kaydet.setPosition(350,220);
	Button Yeniden(sf::Vector2f(300,60), f, "Yeniden Oyna!", 15);
	Yeniden.setPosition(350,700);
	sf::RectangleShape Asfalt(sf::Vector2f(300,800));
	Asfalt.setFillColor(sf::Color(102,102,102));
	Asfalt.setPosition(350,0);
	sf::RectangleShape Cizgi(sf::Vector2f(20,110));
	
	sf::Texture UpKey;
	sf::Texture WKey;
	if(!UpKey.loadFromFile("Up_key.png"))
	{
		MessageBox(NULL, "Bir Dosya Açýlamadý", "Hata", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	if(!WKey.loadFromFile("W_key.png"))
	{
		MessageBox(NULL, "Bir Dosya Açýlamadý", "Hata", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	sf::Sprite UpS(UpKey);
	sf::Sprite WS(WKey);
	
	UpS.setColor(sf::Color::Red);
	WS.setColor(sf::Color::Blue);
	WS.setPosition(150-32,400-32);
	UpS.setPosition(1000-150-32,400-32);
	
	sf::Text O1NT("",f);
	sf::Text O2NT("",f);
	
	sf::CircleShape Oyuncu1,Oyuncu2;
	Oyuncu1.setRadius(40);
	Oyuncu2.setRadius(40);
	
	Oyuncu1.setPosition(375,800-100);
	Oyuncu1.setFillColor(sf::Color::Blue);
	Oyuncu2.setPosition(545,800-100);
	Oyuncu2.setFillColor(sf::Color::Red);
	
	O1NT.setFillColor(sf::Color::Green);
	O2NT.setFillColor(sf::Color::Green);
	
	O1NT.setCharacterSize(10);
	O2NT.setCharacterSize(10);
	
	sf::RectangleShape BitisCizgi(sf::Vector2f(300,20));
	BitisCizgi.setFillColor(sf::Color::Black);
	sf::RectangleShape BitisKare(sf::Vector2f(10,10));
	
	bool WKP = true, UKP = true, win = false;
	
	
	while(window.isOpen())
	{
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
			{
				int msg = MessageBox(NULL, "Web Siteme Bakmak Ýster Misin?", "Kapatmadan Önce", MB_YESNO |MB_ICONQUESTION);
				if(msg == IDYES)
					ShellExecute(0, 0, "http://www.yunusemreaydin.ml", 0, 0 , SW_SHOW );
				window.close();
			}
			if(e.type == sf::Event::KeyReleased)
			{
				if(e.key.code == sf::Keyboard::Up)
					UKP = true;
				if(e.key.code == sf::Keyboard::W)
					WKP = true;
			}
			if(area == 0)
			{
				O1.use(e);
				O2.use(e);
				if(Kaydet.isLeftClicked(window,e))
				{
					bool error = false;
					error += O1.isEmpty();
					error += O2.isEmpty();
					if(error)
						MessageBox(NULL, "Hiçbir Alan Boþ Geçilemez", "Hata", MB_OK | MB_ICONERROR);
					else
					{
						O1N = O1.getString();
						O2N = O2.getString();
						O1NT.setString(O1N);
						O2NT.setString(O2N);
						area = 1;
					}
				}
			}
			if(area == 2)
			{
				if(Yeniden.isLeftClicked(window,e))
				{
					area = 0;
					Oyuncu1.setPosition(375,800-100);
					Oyuncu2.setPosition(545,800-100);
				}
			}
		}
		
		
		window.clear(area == 1 ? sf::Color(207,207,207) : !area ? sf::Color::White : sf::Color(0,100,0));
		
		if(!area)
		{
			window.draw(O1);
			window.draw(O2);
			window.draw(Kaydet);
		}
		else if(area == 1)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && WKP && !win)
			{
				Oyuncu1.move(0,-5);
				WKP = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && UKP && !win)
			{
				Oyuncu2.move(0,-5);
				UKP = false;
			}
			window.draw(Asfalt);
			for(int i = 0; i < 5; i++)
			{
				Cizgi.setPosition(490,(i*50)+50+(i*110));
				window.draw(Cizgi);
			}
			if(Oyuncu1.getPosition().y < 20)
			{
				win = false;
				area = 2;
			}
			else if(Oyuncu2.getPosition().y < 20)
			{
				win = true;
				area = 2;
			}
			window.draw(UpS);
			window.draw(WS);
			window.draw(Oyuncu1);
			window.draw(Oyuncu2);
			O1NT.setPosition(Oyuncu1.getPosition().x+(Oyuncu1.getGlobalBounds().width/2)-(O1NT.getGlobalBounds().width/2), Oyuncu1.getPosition().y-15);
			O2NT.setPosition(Oyuncu2.getPosition().x+(Oyuncu2.getGlobalBounds().width/2)-(O2NT.getGlobalBounds().width/2), Oyuncu2.getPosition().y-15);
			
			BitisCizgi.setPosition(Asfalt.getPosition().x,20);
			window.draw(BitisCizgi);
			for(int i = 0; i < 30; i++)
			{
				BitisKare.setPosition(Asfalt.getPosition().x+(i*10),20+((i%2)*10));
				window.draw(BitisKare);
			}
			
			window.draw(O1NT);
			window.draw(O2NT);
			window.draw(Oyuncu1);
			window.draw(Oyuncu2);
		}
		else if(area == 2)
		{
			sf::Text kazandi("",f);
			kazandi.setString((!win ? O1N : O2N)+" Adlý Oyuncu Oyuncu Kazandý!");
			kazandi.setPosition(window.getSize().x/2-kazandi.getGlobalBounds().width/2,window.getSize().y/2-kazandi.getGlobalBounds().height/2);
			window.draw(kazandi);
			window.draw(Yeniden);
			
		}
		
		window.display();
	}
	return 0;
}
