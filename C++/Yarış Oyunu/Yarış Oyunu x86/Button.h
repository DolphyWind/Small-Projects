#ifndef Button_h
#define Button_h
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <sstream>
using namespace std;

void addStr(string &mainStr, unsigned int after, string afterstr);
void Control(sf::Text &t, sf::RectangleShape rect, int x, unsigned int &c);
string toThere(string str, unsigned int there);
void setToCenter(sf::Text &t, sf::RectangleShape rect);
template <typename T>
string toString(T arg);

namespace Form
{
	class Button : public sf::Drawable
	{
	protected:
		sf::Vector2f position;
		sf::Vector2f Size;
		sf::Text text;
		sf::Font font;
		sf::RectangleShape button;
		bool clicked;
	    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	    unsigned int character;
	    unsigned int cSize;
	    sf::Color c;
	    bool lostFocus;
	public:
		Button();
		Button(sf::Vector2f size, sf::Font &f, string txt, unsigned int charSize);
		sf::Vector2f getPosition();
		void setPosition(sf::Vector2f pos);
		void setPosition(float posX, float posY);
		sf::Vector2f getSize();
		void setSize(sf::Vector2f size);
		void setSize(float sizeX, float sizeY);
		void setText(sf::Text txt);
		void setString(string txt);
		bool isLeftClicked(sf::RenderWindow &window, sf::Event e);
		bool isRightClicked(sf::RenderWindow &window, sf::Event e);
		sf::FloatRect getGlobalBounds();
		sf::FloatRect getLocalBounds();
		sf::FloatRect getTextGlobalBounds();
		sf::FloatRect getTextLocalBounds();
		void setCharacterSize(unsigned int CS);
		unsigned int getCharacterSize();
		void setFont(sf::Font &f);
		const sf::Font *getFont();
		void setButtonColor(sf::Color col);
		sf::Color getButtonColor();
		void setTextColor(sf::Color col);
		sf::Color getTextColor();
	};
}
#endif
