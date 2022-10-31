#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define OPENERROR "Hata"
#include <windows.h>
#include <errno.h>
using namespace std;

template<typename T>
string toString(T arg)
{
	stringstream ss;
	ss<<arg;
	return ss.str();
}

string openFile()
{
	char filename[ MAX_PATH ];

  OPENFILENAME ofn;
    ZeroMemory( &filename, sizeof( filename ) );
    ZeroMemory( &ofn,      sizeof( ofn ) );
    ofn.lStructSize  = sizeof( ofn );
    ofn.hwndOwner    = NULL;  // If you have a window to center over, put its HANDLE here
    ofn.lpstrFilter  = "OGG Dosyalarý (*.ogg)\0*.ogg\0OGX Dosyalarý (*.ogx)\0*.ogx\0WAV Dosyalarý (*.wav)\0*.wav\0FLAC Dosyalarý (*.flac)\0*.flac\0";
    ofn.lpstrFile    = filename;
    ofn.nMaxFile     = MAX_PATH;
    ofn.lpstrTitle   = "Bir Dosya Seç!";
    ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
  
  if(GetOpenFileNameA( &ofn ))
  {
    return filename;
  }
  else
  {
    // All this stuff below is to tell you exactly how you messed up above. 
    // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
    switch (CommDlgExtendedError())
    {
      case CDERR_DIALOGFAILURE   : return "Hata";   break;
      case CDERR_FINDRESFAILURE  : return "Hata";  break;
      case CDERR_INITIALIZATION  : return "Hata";  break;
      case CDERR_LOADRESFAILURE  : return "Hata";  break;
      case CDERR_LOADSTRFAILURE  : return "Hata";  break;
      case CDERR_LOCKRESFAILURE  : return "Hata";  break;
      case CDERR_MEMALLOCFAILURE : return "Hata"; break;
      case CDERR_MEMLOCKFAILURE  : return "Hata";  break;
      case CDERR_NOHINSTANCE     : return "Hata";     break;
      case CDERR_NOHOOK          : return "Hata";          break;
      case CDERR_NOTEMPLATE      : return "Hata";      break;
      case CDERR_STRUCTSIZE      : return "Hata";      break;
      case FNERR_BUFFERTOOSMALL  : return "Hata";  break;
      case FNERR_INVALIDFILENAME : return "Hata"; break;
      case FNERR_SUBCLASSFAILURE : return "Hata"; break;
      default                    : return "Hata";
    }
  }
}
string pop_back(string &str)
{
	str = str.substr(0,str.length()-1);
}
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Turkish");
	sf::RenderWindow window(sf::VideoMode(400,400),"OGG Player",sf::Style::Close);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	char myPath[_MAX_PATH+1];
	GetModuleFileName(NULL,myPath,_MAX_PATH);
	string progpath = myPath;
	
	for(;;)
	{
		if(progpath[progpath.length()-1] == '\\')
			break;
		else
			progpath = progpath.substr(0,progpath.length()-1);
	}
	
	
	
	

	sf::Image icon;
	if(!icon.loadFromFile(progpath+"images/icon.png"))
	{
		MessageBox(NULL,"Bir dosyaya ulaþýlamadý!","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
	
	float ses=15;
	
	
	
	string file="";
	bool Opened = false;
	if (argc > 1)
	{
		file = argv[1];
		Opened = true;
	}
	
	bool stopped=false;
	sf::Music M;
	if(Opened)
	{
		M.openFromFile(file);
		M.play();
		M.setVolume(ses);
	}
	bool paused = false;
	
	sf::Time MusicTime;
	sf::Font f;
	if(!f.loadFromFile(progpath+"Font.ttf"))
	{
		MessageBox(NULL,"Bir dosyaya ulaþýlamadý!","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	sf::Text txt("",f);
	string str;
	bool lostFocus=false;
	sf::Texture Continue,Pause,Vol;
	if(!Continue.loadFromFile(progpath+"images/Continue.png"))
	{
		MessageBox(NULL,"Bir dosyaya ulaþýlamadý!","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	if(!Pause.loadFromFile(progpath+"images/Pause.png"))
	{
		MessageBox(NULL,"Bir dosyaya ulaþýlamadý!","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	if(!Vol.loadFromFile(progpath+"images/Volume.png"))
	{
		MessageBox(NULL,"Bir dosyaya ulaþýlamadý!","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	
	
	sf::Texture On,Off,Back;
	if(!On.loadFromFile(progpath+"images/On.png"))
	{
		MessageBox(NULL,"Bir dosyaya ulaþýlamadý!","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	if(!Off.loadFromFile(progpath+"images/Off.png"))
	{
		MessageBox(NULL,"Bir dosyaya ulaþýlamadý!","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	if(!Back.loadFromFile(progpath+"images/Back.png"))
	{
		MessageBox(NULL,"Bir dosyaya ulaþýlamadý!","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	sf::Sprite OnS(On),OffS(Off),BackS(Back);
	BackS.setPosition(0,350);
	OnS.setPosition(150,350);
	OffS.setPosition(150,350);
	
	sf::Sprite ConS(Continue),PauseS(Pause),VolS(Vol);
	ConS.setPosition(200,400-49);
	PauseS.setPosition(200,400-49);
	VolS.setPosition(250,350);
	sf::Vector2i ms;
	txt.setColor(sf::Color::Black);
	txt.setCharacterSize(15);
	
	sf::Clock c;
	sf::Time t;
	bool click = true;
	bool press = true;
	bool muted = false;
	bool finished = false;
	
	sf::Clock mc;
	sf::Time tc;
	sf::Text Controls("Kontroller: \nM: Ses Kýs/Aç\nL: Tekrarla Açýk/Kapalý\nR : Yeniden baþlat\nCTRL+O: Yeni Bir Dosya Aç\nSpace: Durdur/Devam Et\nYukarý Ok/Aþaðý Ok: Ses Arttýr/Ses Azalt\nSol Ok/Sað Ok: Müziði Geri / Ýleri Sar\n\nYunus Emre Aydýn Tarafýndan Yapýlmýþtýr.",f);
	Controls.setCharacterSize(15);
	Controls.setColor(sf::Color(0,0,0));
	
	sf::RectangleShape Siyah(sf::Vector2f(100,49));
	Siyah.setFillColor(sf::Color::Black);
	Siyah.setPosition(300,351);
	sf::RectangleShape KMor(sf::Vector2f(ses,49));
	KMor.setFillColor(sf::Color(148,0,211));
	KMor.setPosition(300,351);
	sf::RectangleShape MorA(sf::Vector2f(10,49));
	MorA.setFillColor(sf::Color(147,112,219));
	MorA.setPosition(300 + ses,351);
	sf::RectangleShape Black(sf::Vector2f(100,49));
	Black.setFillColor(sf::Color::Black);
	Black.setPosition(50,351);
	sf::RectangleShape KirmiziA(sf::Vector2f(0,49));
	KirmiziA.setPosition(50,351);
	KirmiziA.setFillColor(sf::Color(255,0,0));
	sf::RectangleShape KKirmizi(sf::Vector2f(10,49));
	KKirmizi.setFillColor(sf::Color(200,0,50));
	KKirmizi.setPosition(50,351);
	txt.setPosition(70,365);
	txt.setColor(sf::Color(144,138,84));
	sf::Text Loop("Tekrarla",f);
	Loop.setColor(sf::Color(0,0,0));
	Loop.setCharacterSize(15);
	
	sf::RectangleShape YK(sf::Vector2f(50,49));
	YK.setFillColor(sf::Color(255,0,0));
	YK.setPosition(150,351);
	Loop.setPosition(145,330);
	
  	while(window.isOpen())
  	{
  		sf::Event e;
  		while(window.pollEvent(e))
  		{
  			if(e.type == sf::Event::Closed)
				window.close();								
  			if(e.type == sf::Event::LostFocus)  			
  				lostFocus=true;			
  			if(e.type == sf::Event::GainedFocus)
  				lostFocus=false;
		}
		ms = sf::Mouse::getPosition(window);
		t = c.getElapsedTime();
		tc = mc.getElapsedTime();
  		str = "";
  		if(Opened)
  		{
  			MusicTime = M.getPlayingOffset();
  			int Msecond = MusicTime.asSeconds();
  			if(Msecond/3600<10)
  				str+="0";
  			str+=toString(Msecond/3600);
  			int saat = Msecond/3600;
  			str+=":";
  			if(Msecond/60-saat*60<10)
  				str +="0"; 
  			str+=toString(Msecond/60-saat*60);
  			str+=":";
  			if(Msecond%60<10)
  				str+="0";
  			str+=toString(Msecond%60);
  			txt.setString(str); 
						
		}
  			
  			
  		
		if(!lostFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			{
				if(!Opened)
				{
					file = openFile();
					if(file != OPENERROR)
					{
						M.openFromFile(file);
						Opened = true;
						M.play();	
					}							
					else
					{
						MessageBox(NULL,"Dosya Açýlýrken Bir Hata Oluþtu","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);						
					}				
				}
				else
				{
					M.pause();
					file = openFile();
					if(file != OPENERROR)
					{
						M.openFromFile(file);
						M.play();
						Opened = true;
					}							
					else
					{
						MessageBox(NULL,"Dosya Açýlýrken Bir Hata Oluþtu","Dosya Açýlamadý!",MB_OK | MB_ICONERROR);						
					}				
				}
				M.setVolume(ses);
			}
	
		}
		
		if(!click)
		{
			if(t>sf::seconds(0.25))
			{
				click = true;
			}
		}
		
		if(click && !lostFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && Opened)
		{
			if(!paused)
			{
				paused = true;
				M.pause();
			}
			else
			{
				paused = false;
				M.play();
			}
			click = false;
			c.restart();
		}
		if(click && !lostFocus && Opened && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			M.setPlayingOffset(M.getPlayingOffset()+sf::seconds(5));
			if(M.getPlayingOffset()>M.getDuration())
				M.setPlayingOffset(M.getDuration());
			click = false;
			c.restart();
		}
			
		if(click && !lostFocus && Opened && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			M.setPlayingOffset(M.getPlayingOffset()+sf::seconds(-5));
			if(M.getPlayingOffset()<sf::seconds(0))
				M.setPlayingOffset(sf::seconds(0));
			click = false;
			c.restart();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::M) && !lostFocus && Opened && click)
		{
			if(muted)
			{
				M.setVolume(ses);
				muted = false;
				KMor.setSize(sf::Vector2f(ses,49));
				MorA.setPosition(300+ses-5,351);
			}
			else if(!muted)
			{
				M.setVolume(0);
				muted = true;
				KMor.setSize(sf::Vector2f(0,49));
				MorA.setPosition(300,351);
			}
			click = false;
			c.restart();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !lostFocus && Opened && click)
		{
			M.setLoop(!M.getLoop());
			click = false;
			c.restart();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !lostFocus && Opened && click)
		{
			M.setPlayingOffset(sf::seconds(0));
			click = false;
			c.restart();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !lostFocus && Opened && click)
		{
			muted = false;
			ses++;
			if(ses>100)ses = 100;
			KMor.setSize(sf::Vector2f(ses,49));
			MorA.setPosition(300+ses-5,351);
			M.setVolume(ses);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !lostFocus && Opened && click)
		{
			muted = false;
			ses--;
			if(ses<0)ses = 0;
			KMor.setSize(sf::Vector2f(ses,49));
			MorA.setPosition(300+ses-5,351);
			M.setVolume(ses);
		}
		if(click)
		{
			c.restart();
		}
		
		
		if(!press)
		{
			if(tc>sf::seconds(0.25))
				press = true;
		}
		if(!lostFocus && sf::Mouse::isButtonPressed(sf::Mouse::Left) && Opened && press)
		{
			if(ms.x>=200 && ms.x<=250 && ms.y>=351 && ms.y<=400)
			{
				if(!paused)
				{
					paused = true;
					M.pause();
				}
				else
				{
					
					paused = false;
					M.play();
				}
				press = false;
				mc.restart();
			}
			if(ms.x>=250 && ms.x<=300 && ms.y>=350 && ms.y<=400)
			{
				if(muted)
				{
					M.setVolume(ses);
					muted = false;
					KMor.setSize(sf::Vector2f(ses,49));
					MorA.setPosition(300+ses-5,351);
				}
				else if(!muted)
				{
					M.setVolume(0);
					muted = true;
					KMor.setSize(sf::Vector2f(0,49));
					MorA.setPosition(300,351);
				}
				press = false;
				mc.restart();
			}
			if(ms.x>=150 && ms.x<=200 && ms.y>=350 && ms.y<=400)
			{
				M.setLoop(!M.getLoop());
				press = false;
				mc.restart();
			}
			if(ms.x>=0 && ms.x<=50 && ms.y>=350 && ms.y<=400)
			{
				M.setPlayingOffset(sf::seconds(0));
				press = false;
				mc.restart();
			}
		}
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !lostFocus && Opened)
		{
			if(ms.x>=300 && ms.x<=400 && ms.y>=350 && ms.y<=400)
			{
				muted = false;
				ses = ms.x-300;
				KMor.setSize(sf::Vector2f(ses,49));
				MorA.setPosition(300+ses-5,351);
				M.setVolume(ses);
			}
			if(ms.x>=50 && ms.x<=150 && ms.y>=351 && ms.y<=400)
			{
				M.setPlayingOffset(sf::seconds(M.getDuration().asSeconds()/100.0*(ms.x-50)));
				KirmiziA.setSize(sf::Vector2f(ms.x-50,49));
				KKirmizi.setPosition(50+KirmiziA.getSize().x-5,351);
			}
		}
		
		if(press)
			mc.restart();
			
		
			
		if(ms.x>=300 && ms.x<=400 && ms.y>=350 && ms.y<=400 && Opened && !lostFocus)
			SetCursor(LoadCursor(NULL,IDC_HAND));
		if(ms.x>=200 && ms.x<=250 && ms.y>=351 && ms.y<=400 && Opened && !lostFocus)
			SetCursor(LoadCursor(NULL,IDC_HAND));
		if(ms.x>=250 && ms.x<=300 && ms.y>=350 && ms.y<=400 && Opened && !lostFocus)
			SetCursor(LoadCursor(NULL,IDC_HAND));
		if(ms.x>=50 && ms.x<=150 && ms.y>=351 && ms.y<=400 && Opened && !lostFocus)
			SetCursor(LoadCursor(NULL,IDC_HAND));
		if(ms.x>=150 && ms.x<=200 && ms.y>=350 && ms.y<=400 && Opened && !lostFocus)
			SetCursor(LoadCursor(NULL,IDC_HAND));
		if(ms.x>=0 && ms.x<=50 && ms.y>=350 && ms.y<=400 && Opened && !lostFocus)
			SetCursor(LoadCursor(NULL,IDC_HAND));
		
		
		if(Opened)
		{
			window.clear(sf::Color::White);
			if(M.getStatus() == sf::Music::Stopped)
			{
				if(!M.getLoop())
					paused = true;
			}
			if(paused)
			{	
				window.draw(PauseS);
			}
			
			else if(!paused)
			{
				window.draw(ConS);
			}
			KirmiziA.setSize(sf::Vector2f(M.getPlayingOffset().asSeconds()/(M.getDuration().asSeconds()/100.0),49));
			KKirmizi.setPosition(50+KirmiziA.getSize().x-5,351);			
			window.draw(VolS);
			window.draw(Siyah);
			window.draw(KMor);
			window.draw(MorA);
			window.draw(Black);
			window.draw(KirmiziA);
			window.draw(KKirmizi);
			window.draw(txt);
			window.draw(YK);
			if(!M.getLoop())
				window.draw(OffS);
			else
				window.draw(OnS);
			window.draw(Loop);
			window.draw(BackS);
			window.draw(Controls);
		}
		if(!Opened)
		{
			window.clear(sf::Color(255,0,0));
			sf::Text Open("\"CTRL+O\"\ Ýle Dosya Aç",f);
			Open.setPosition((window.getSize().x - Open.getGlobalBounds().width) / 2, 175);
			window.draw(Open);
		}	
		
		window.display();
	}

	return 0;
}
