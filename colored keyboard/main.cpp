#include <Windows.h>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

void pressScroll()
{
	INPUT ip;
 
    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
 
    // Press the "A" key
    ip.ki.wVk = VK_SCROLL; // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));
 
    // Release the "A" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

int main()
{
	bool kb = false, press = false;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Test Area", sf::Style::Close);
	window.setFramerateLimit(60);
	
	while(window.isOpen())
	{
		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed) window.close();
		}
		for(int i = 0; i < 101; i++)
		{
			sf::Keyboard::Key key = (sf::Keyboard::Key)i;
			if(sf::Keyboard::isKeyPressed(key))
			{
				kb = true;
				break;
			}
			kb = false;
			if(press)
			{
				pressScroll();
				press = false;
			}
			
		}
		if(kb && !press)
		{
			pressScroll();
			press = true;
		}
		
		window.clear();
		
		window.display();
	}
    
    return 0;
}
