#ifndef TextBox_h
#define TextBox_h
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <sstream>
using namespace std;

namespace Form
{
	class TextBox : public sf::Drawable
	{
	private:
		sf::RenderWindow *wndPtr;
		sf::Font *fontPtr;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::RectangleShape TB;
		sf::Text text;
		sf::Text hintText;
		string yedek;
		unsigned int maxChar;
		bool focus;
		bool drawHint;
		bool leftPress;
		unsigned int NOCIT;
		bool tas;
		bool first;
		bool onlyNumbers;
		bool lostFocus;
	public:
		TextBox(sf::RenderWindow &window, sf::Font &font);
		TextBox(sf::RenderWindow &window, sf::Font &font, float sizeX, string hint, unsigned int mC);
		void use(sf::Event e);
		void setText(sf::Text txt);
		void setText(string txt);
		sf::Text getText();
		string getString();
		void setHintString(string hint);
		void setHintText(sf::Text hint);
		sf::Text getHintText();
		string getHintString();
		void setPosition(float posX, float posY);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition();
		void setMaximumCharacter(unsigned int mC);
		unsigned int getMaximumCharacter();
		void setSize(float sizeX);
		float getSize();
		bool getFocus();
		bool isEmpty();
		void OnlyNumbers(bool ON);
		bool getOnlyNumbers();
	};
}

#endif
