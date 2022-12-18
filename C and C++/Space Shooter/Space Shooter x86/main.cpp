#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdlib>
#include <Windows.h>
#include <sstream>
#include <ctime>
#include "game.h"
#include "Collision.cpp"
using namespace std;

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Turkish");
	
	//Opening Window
	sf::RenderWindow window(sf::VideoMode(1000,800),"Space Shooter",sf::Style::Close);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	
	//Booleans
	bool mainmenu = true;
	bool ButtonClick = true;
	bool quit = false;
	bool isAnyMB = false;
	bool SAF = false;
	bool lostFocus = false;
	bool died = false;
	bool boomAnimationPlayed = false;
	bool paused = false;
	bool healmove = false;
	bool canGetDamageable = true;
	
	//Textures
	sf::Texture MainBack;
	sf::Texture signT;
	sf::Texture PlayerT;
	sf::Texture EnemyT;
	sf::Texture BulletT;
	sf::Texture GameBack;
	sf::Texture Boom;
	sf::Texture Heal;
	
	//Musics
	sf::Music mm;
	sf::Music inGameMusic;
	
	//Clocks
	sf::Clock EnemyClock;
	sf::Clock HitClock;
	sf::Clock BulletClock;
	sf::Clock ButtonClock;
	sf::Clock HealClock;
	sf::Clock DamageClock;
	
	//Times
	sf::Time EnemyTimer;
	sf::Time ButtonTimer;
	sf::Time HitTimer;
	sf::Time BulletTimer;
	sf::Time HealTimer;
	sf::Time DamageTimer;
	
	//Fonts
	sf::Font font;
	sf::Font PixelFont;
	
	//Ints
	unsigned int score = 0;
	int boomX = 0,boomY = 0;
	int boomanimtimer = 0;
	
	//Vector
	vector<Enemy> enemies;
	
	//Vector2i
	sf::Vector2i ms;//For Mouse Position
	
	//Images
	sf::Image sign;
	sf::Image icon;

	//Opening Musics
	if(!mm.openFromFile("Musics/MainMenu.wav"))
	{
		MessageBox(NULL,"\"Musics/MainMenu.wav\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	if(!inGameMusic.openFromFile("Musics/inGame.wav"))
	{
		MessageBox(NULL,"\"Musics/inGame.wav\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	//Opening Images
	if(!sign.loadFromFile("Textures/Sign.png"))
	{
		MessageBox(NULL,"\"Textures/Sign.png\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	if(!icon.loadFromFile("Textures/icon.png"))
	{
		MessageBox(NULL,"\"Textures/icon.png\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	//Opening Textures
	if(!PlayerT.loadFromFile("Textures/SpaceShip.png"))
	{
		MessageBox(NULL,"\"Textures/SpaceShip.png\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	if(!EnemyT.loadFromFile("Textures/Enemy.png"))
	{
		MessageBox(NULL,"\"Textures/Enemy.png\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	if(!BulletT.loadFromFile("Textures/Bullet.png"))
	{
		MessageBox(NULL,"\"Textures/Bullet.png\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}	
	if(!GameBack.loadFromFile("Textures/GameBack.png"))
	{
		MessageBox(NULL,"\"Textures/GameBack.png\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	if(!MainBack.loadFromFile("Textures/MainBack.jpg"))
	{
		MessageBox(NULL,"\"Textures/MainBack.jpg\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	if(!Boom.loadFromFile("Textures/Boom.png"))
	{
		MessageBox(NULL,"\"Textures/Boom.png\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	if(!Heal.loadFromFile("Textures/Heal.png"))
	{
		MessageBox(NULL,"\"Textures/Heal.png\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
		
	//Opening Fonts
	if(!font.loadFromFile("Fonts/Font.ttf"))
	{
		MessageBox(NULL,"\"Fonts/Font.ttf\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	if(!PixelFont.loadFromFile("Fonts/PixelFont.ttf"))
	{
		MessageBox(NULL,"\"Fonts/PixelFont.ttf\" is wouldn\'t be opened.","Error",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	//Player
	Player player(&PlayerT);
	player.ship.setPosition(0,365.5);

	//Texts
	sf::Text ScrText("",font);//Score Text
	sf::Text HPText("",PixelFont);//HP Text
	
	//Transform
	sf::Transform transform;
	
	//Texture Settings
	signT.loadFromImage(sign);
	
	//Sprites
	sf::Sprite GameBackS(GameBack);
	sf::Sprite MainBackS(MainBack);
	sf::Sprite signS(signT);
	sf::Sprite HealS(Heal);
		
	//Sprites' settings
	signS.setPosition(window.getSize().x/2-sign.getSize().x/2,-(int)sign.getSize().y);
	HealS.setPosition(1000,rand()%736);
	
	//Musics' settings
	mm.setVolume(9);
	mm.play();
	mm.setLoop(true);
	inGameMusic.setLoop(true);
	inGameMusic.setVolume(9);
	
	//Transform's setting
	transform.rotate(90,50,400);
	
	//Buttons and Buttons' settings
	Button play(sf::Vector2f(300,60),"Play!",font);
	play.t.setCharacterSize(20);
	play.t.setColor(sf::Color::Red);
	play.btn.setFillColor(sf::Color(0,191,255));
	play.setPosition(sf::Vector2f(350,420));
	
	Button exit(sf::Vector2f(300,60),"Exit The Game",font);
	exit.t.setCharacterSize(20);
	exit.t.setColor(sf::Color::Red);
	exit.btn.setFillColor(sf::Color(0,191,255));
	exit.setPosition(sf::Vector2f(350,630));
	
	Button programmer(sf::Vector2f(300,60),"Programmer",font);
	programmer.t.setCharacterSize(20);
	programmer.t.setColor(sf::Color::Red);
	programmer.btn.setFillColor(sf::Color(0,191,255));
	programmer.setPosition(sf::Vector2f(350,560));
	
	Button Controls(sf::Vector2f(300,60),"Controls",font);
	Controls.t.setCharacterSize(20);
	Controls.t.setColor(sf::Color::Red);
	Controls.btn.setFillColor(sf::Color(0,191,255));
	Controls.setPosition(sf::Vector2f(350,490));	//Like A Poem Right? ;D
	
	//Text Settings
	HPText.setCharacterSize(15);
	
	//MessageBoxes
	MB mbexit("Do You Really Want To Exit?\n\n\n",YESNO,window,font);
	MB mbprog("The Game\'s Programmer:\n\t\t\t\t\tYunus Emre Aydýn",ONLYOK,window,font);
	MB mbcont("W-UpKey: Up\nS-DownKey: Down\nA-LeftKey: Left\nD-RightKey: Right\nSpace: Shoot\n\nDont Get Killed By Enemies!",ONLYOK,window,font);
	MB mbDie("You Died. Your Score Was: ",ONLYOK,window,font);
	MB mbpause("Paused! Click OK To Play",ONLYOK,window,font);
	
	//Sets The Window Icon
	window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());

	while(window.isOpen())
	{
		//Events
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed || quit)
				window.close();
			if(e.type == sf::Event::GainedFocus)
				lostFocus = false;
			if(e.type == sf::Event::LostFocus)
			{
				lostFocus = true;
				if(!mainmenu && !died)
					paused = true;
			}
				
		}
		boomanimtimer ++;
		if(boomanimtimer>=100)
			boomanimtimer = 0;
//Checks if Focus isn't lostw
if(!lostFocus && !paused)
{		
		//Mouse Position
		ms = sf::Mouse::getPosition(window);

		//Getting Elapsed Time
		ButtonTimer = ButtonClock.getElapsedTime();
		EnemyTimer = EnemyClock.getElapsedTime();
		HitTimer = HitClock.getElapsedTime();
		BulletTimer = BulletClock.getElapsedTime();
		HealTimer = HealClock.getElapsedTime();
		
		
		//Check Is Any MessageBox on the window
		if(mbexit.appear || mbprog.appear || mbcont.appear || mbDie.appear || mbpause.appear)
			isAnyMB = true;
		else
		 	isAnyMB = false;
		 	
		//if Mouse Can Click The Buttons Changes The Outline Colors etc.
		if(!isAnyMB && mainmenu)
		{
			play.ChangeCur(window);
			exit.ChangeCur(window);
			programmer.ChangeCur(window);
			Controls.ChangeCur(window);
		}
		
		//Button Click Timer
		if(!ButtonClick)
		{
			if(ButtonTimer>sf::seconds(0.5))
				ButtonClick = true;
		}
		
		//Click Events
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && ButtonClick && !isAnyMB && !lostFocus)
		{
			if(exit.CanClickable(ms) && mainmenu && SAF)
			{
				mbexit.appear = true;
				ButtonClick = false;
			}
			if(programmer.CanClickable(ms) && mainmenu && SAF)
			{
				mbprog.appear = true;
				ButtonClick = false;
			}
			if(play.CanClickable(ms) && mainmenu && SAF)
			{
				mainmenu = false;
				inGameMusic.play();
				ButtonClick = false;
				EnemyClock.restart();
			}
			if(Controls.CanClickable(ms) && mainmenu && SAF)
			{
				mbcont.appear = true;
				ButtonClick = false;
			}
			if(!SAF)
			{
				SAF = true;
				signS.setPosition(signS.getPosition().x,0);
			}
			
			ButtonClock.restart();
		}
		if(ButtonClick)
			ButtonClock.restart();		
		
		//Key Events
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && !mainmenu && !lostFocus && !died)
				player.ship.move(-5,0);
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && !mainmenu && !lostFocus && !died)
				player.ship.move(5,0);
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && !mainmenu && !lostFocus && !died)
				player.ship.move(0,-7.5);	
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && !mainmenu && !lostFocus && !died)
				player.ship.move(0,7.5);
		
		//Checks The Ships Position
		if(player.ship.getPosition().x <= 0 && !died)player.ship.setPosition(0,player.ship.getPosition().y);
		if(player.ship.getPosition().y <= 0 && !died)player.ship.setPosition(player.ship.getPosition().x,0);
		if(player.ship.getPosition().x+100 >= 1000 && !died) player.ship.setPosition(900,player.ship.getPosition().y);
		if(player.ship.getPosition().y+69 >= 800 && !died) player.ship.setPosition(player.ship.getPosition().x,731);
		
		//Spawns A Bullet
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !mainmenu && !lostFocus && !died)
		{			
			if(BulletTimer>sf::seconds(0.5))
			{
				player.bullets.push_back(Bullet(&BulletT,sf::Vector2f(player.ship.getPosition().x+50,player.ship.getPosition().y+37-sf::Sprite(BulletT).getGlobalBounds().height/2)));
				BulletClock.restart();
			}		
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !mainmenu && !died)
		{
			paused = true;
		}
			
		//Spawns An Enemy
		if(EnemyTimer>sf::seconds(0.3) && !mainmenu)
		{
			float anything = rand()%700;
			enemies.push_back(Enemy(&EnemyT,sf::Vector2f(1000,anything)));
			EnemyClock.restart();
		}
		
		//Deletes A Bullet While Be Useless
		for(int i=0;i<player.bullets.size();i++)
		{
			player.bullets[i].bullet.move(7.5,0);
			if(player.bullets[i].bullet.getPosition().x>1000)
				player.bullets.erase(player.bullets.begin()+i);
		}
		
		//Deletes An Enemy While Be Useless
		for(int i=0;i<enemies.size();i++)
		{
			if(!enemies[i].killed)
				enemies[i].ship.move(-5,0);
			if(enemies[i].ship.getPosition().x<-100)			
				enemies.erase(enemies.begin()+i);	
				
			//Checks if An Enemy Be Killed
			for(int j=0;j<player.bullets.size();j++)
			{
				if(Collision::PixelPerfectTest(enemies[i].ship,player.bullets[j].bullet) && !enemies[i].killed)
				{
					enemies[i].HP-=5;
					if(enemies[i].HP<=0)
					{
						score+=10;
						enemies[i].killed = true;
					}
					player.bullets.erase(player.bullets.begin()+j);
				}
			}
			
			//Checks if Player And An Enemy Was collided
			if(Collision::PixelPerfectTest(enemies[i].ship,player.ship) && !died && !enemies[i].killed)
			{
				if(canGetDamageable)
				{
					player.HP-=5;
					canGetDamageable = false;
				}
				enemies[i].killed = true;
			}
		}
		
		//Add Some invincible
		if(!canGetDamageable)
		{
			DamageTimer = DamageClock.getElapsedTime();
		}
		else
			DamageClock.restart();
		if(DamageTimer>=sf::seconds(1.5))
			canGetDamageable = true;
		
		//Heal Spawn and Move
		if(HealTimer>sf::seconds(20))
		{
			healmove = true;
			HealClock.restart();
		}
		if(healmove)
			HealS.move(-5,0);
		
		//Checks if player and heart is collided
		if(Collision::PixelPerfectTest(HealS,player.ship) && !died)
		{
			player.HP+= 5;
			if(player.HP>player.HPMAX) player.HP = 15;
			HealS.setPosition(1000,rand()%736);
			healmove = false;
		}
		
		if(HealS.getPosition().x<=-64)
		{
			healmove = false;
			HealS.setPosition(1000,rand()%736);
		}
		
		//Checks if Player Died
		if(player.HP<=0)
		{
			died = true;
			player.ship.setTexture(Boom);
			player.ship.setTextureRect(sf::IntRect(boomX*64,boomY*64,64,64));
			boomX++;
			if(boomX>=6)
			{
				boomX = 0;
				boomY ++;
			}
			if(boomY >= 4)
			{
				boomAnimationPlayed = true;
				player.ship.setPosition(9999,9999);
			}
		}
		if(died)
		{
			mbDie.txt.setString("You Died! Your Score Was: "+toString(score));
			if(boomAnimationPlayed)
				mbDie.appear = true;
		}
		
		//Sign's Fall
		if(!SAF)
		{
			signS.setPosition(signS.getPosition().x,signS.getPosition().y+3);
		}
		if(signS.getPosition().y>=0 && !SAF)
		{
			signS.setPosition(signS.getPosition().x,0);
			SAF = true;
		}
		
		//Score Text
		ScrText.setString("Your Score: "+toString(score));
		
		//Clears The Window
		window.clear();
		
		//if Player is On To The Main Menu Game'll Draws This
}
else if(!lostFocus && paused)
{
	//if Paused And MessageBox isn't Drawn. That'll Draws it
	if(!mbpause.appear)mbpause.appear = true;
}
		if(mainmenu)
		{
			//Draws Main Menu Background
			window.draw(MainBackS);
			
			//Draws Buttons
			play.draw(window);
			exit.draw(window);
			programmer.draw(window);
			Controls.draw(window);
			
			//Draws Sign
			window.draw(signS);
			
			//Checks Message Boxes
			int newmsg = mbprog.draw(window);
			if(newmsg == BTOK)
			{
				mbprog.appear = false;
				ButtonClick = false;
			}				
			int msg = mbexit.draw(window);
			if(msg == BTYES)
			{
				quit = true;
				ButtonClick = false;
			}				
			if(msg == BTNO)
			{
				mbexit.appear = false;
				ButtonClick = false;
			}			
			int newnewmsg = mbcont.draw(window);
			if(newnewmsg == BTOK)
			{
				mbcont.appear = false;
				ButtonClick = false;
			}
				
		}
		
		//if Player isn't On To The Main Menu Game'll Draws This
		else
		{
			//Draws Background
			window.draw(GameBackS);
			
			//Stops The Main Menu Music
			mm.stop();
			
			//Draws Bullets
			for(int i=0;i<player.bullets.size();i++)
				window.draw(player.bullets[i].bullet);
			
			//Draws Enemies and his HPs' (if Enenmy Was Killed Draws Booming Animation)
			for(int i=0;i<enemies.size();i++)
			{
				HPText.setString(toString(enemies[i].HP)+"/"+toString(enemies[i].HPMAX));
				HPText.setPosition(enemies[i].ship.getPosition().x+50-HPText.getGlobalBounds().width/2,enemies[i].ship.getPosition().y-HPText.getGlobalBounds().height);
				if(enemies[i].killed)
				{
					enemies[i].ship.setTexture(Boom);
					enemies[i].ship.setTextureRect(sf::IntRect(enemies[i].BOOMX*64,enemies[i].BOOMY*64,64,64));
					enemies[i].BOOMX++;
					if(enemies[i].BOOMX>6)
					{
						enemies[i].BOOMY++;
						enemies[i].BOOMX = 0;
					}
					if(enemies[i].BOOMY>4)
						enemies.erase(enemies.begin()+i);
				}
				window.draw(enemies[i].ship);
				
				if(!enemies[i].killed)
					window.draw(HPText);
			}
			
			//Draws Die Message Box And Checks The input
			int diemsg = mbDie.draw(window);
			if(diemsg == BTOK)
				quit = true;
				
			//Draws the Pause Message Box 
			int mbp = mbpause.draw(window);
			if(mbp == BTOK)
			{
				paused = false;
				mbpause.appear = false;
			}
			
			//Draws Player And HP
			HPText.setString(toString(player.HP)+"/"+toString(player.HPMAX));
			HPText.setPosition(player.ship.getPosition().x+50-HPText.getGlobalBounds().width/2,player.ship.getPosition().y-HPText.getGlobalBounds().height);
			if(!canGetDamageable && !died)
			{
				if(rand()%2)
				{
					window.draw(player.ship);	
					window.draw(HPText);
				}
			}
			else
			{
				window.draw(player.ship);	
				if(!died)window.draw(HPText);
			}	
			
			//Draws Score Text
			window.draw(ScrText);
			
			//Draws Heal Heart
			window.draw(HealS);
		}

	
		//Displays The Window
		window.display();
	}

	return 0;
}
