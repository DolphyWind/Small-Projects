#include "Textbox.h"

int enaz0(int a)
{
	if(a > 0)
		return a;
	return 0;
}

Form::TextBox::TextBox(sf::RenderWindow &window, sf::Font &font)
{
	wndPtr = &window;
	fontPtr = &font;
	hintText.setFont(font);
	text.setFont(font);
	text.setCharacterSize(15);
	hintText.setCharacterSize(15);
	drawHint = true;
	text.setPosition(5,5);
	hintText.setPosition(5,5);
	TB.setOutlineColor(sf::Color::Black);
	TB.setOutlineThickness(1);
	TB.setFillColor(sf::Color(235,235,235));
	text.setFillColor(sf::Color::Black);
	hintText.setFillColor(sf::Color(140,140,140));
	leftPress = true;
	focus = false;
	NOCIT = 0;
	first = true;
	onlyNumbers = false;
	lostFocus = false;
}
Form::TextBox::TextBox(sf::RenderWindow &window, sf::Font &font, float sizeX, string hint, unsigned int mC)
{
	wndPtr = &window;
	fontPtr = &font;
	hintText.setFont(font);
	hintText.setString(hint);
	text.setFont(font);
	text.setCharacterSize(15);
	hintText.setCharacterSize(15);
	if(sizeX < 150) sizeX = 150;
	TB.setSize(sf::Vector2f(sizeX,30));
	maxChar = mC;
	drawHint = true;
	text.setPosition(5,5);
	hintText.setPosition(5,5);
	TB.setOutlineColor(sf::Color::Black);
	TB.setOutlineThickness(1);
	TB.setFillColor(sf::Color(235,235,235));
	text.setFillColor(sf::Color::Black);
	hintText.setFillColor(sf::Color(140,140,140));
	leftPress = true;
	focus = false;
	NOCIT = 0;
	first = true;
	onlyNumbers = false;
	lostFocus = false;
}
void Form::TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(TB, states);
	
	if(drawHint)
		target.draw(hintText, states);
	target.draw(text, states);
}
void Form::TextBox::setText(sf::Text txt)
{
	string TexT = txt.getString();
	for(int i = 0; i < TexT.length(); i++)
	{
		if(TexT[i] == '\n')
			TexT[i] = ' ';
	}
	txt.setString(TexT);
	text.setString(txt.getString());
}
void Form::TextBox::setText(string txt)
{
	for(int i = 0; i < txt.length(); i++)
	{
		if(txt[i] == '\n')
			txt[i] = ' ';
	}
	text.setString(txt);
}
sf::Text Form::TextBox::getText()
{
	return sf::Text(yedek,*fontPtr);
}
string Form::TextBox::getString()
{
	return yedek;
}
void Form::TextBox::setHintString(string hint)
{
	for(int i = 0; i < hint.length(); i++)
	{
		if(hint[i] == '\n')
			hint[i] = ' ';
	}
	hintText.setString(hint);
}
void Form::TextBox::setHintText(sf::Text hint)
{
	string TexT = hint.getString();
	for(int i = 0; i < TexT.length(); i++)
	{
		if(TexT[i] == '\n')
			TexT[i] = ' ';
	}
	hint.setString(TexT);
	hintText.setString(hint.getString());
}
sf::Text Form::TextBox::getHintText()
{
	return hintText;
}
string Form::TextBox::getHintString()
{
	return hintText.getString();
}
void Form::TextBox::setPosition(float posX, float posY)
{
	TB.setPosition(posX,posY);
	text.setPosition(posX+5, posY+5);
	hintText.setPosition(posX+5, posY+5);
}
void Form::TextBox::setPosition(sf::Vector2f pos)
{
	TB.setPosition(pos.x,pos.y);
	text.setPosition(pos.y+5, pos.y+5);
	hintText.setPosition(pos.x+5, pos.y+5);
}
sf::Vector2f Form::TextBox::getPosition()
{
	return TB.getPosition();
}
void Form::TextBox::setMaximumCharacter(unsigned int mC)
{
	maxChar = mC;
}
unsigned int Form::TextBox::getMaximumCharacter()
{
	return maxChar;
}
void Form::TextBox::setSize(float sizeX)
{
	if(sizeX < 150) sizeX = 150;
	TB.setSize(sf::Vector2f(sizeX,30));
}
float Form::TextBox::getSize()
{
	return TB.getSize().x;
}
void Form::TextBox::use(sf::Event e)
{
	if(e.type == sf::Event::LostFocus)
		lostFocus = true;
	if(e.type == sf::Event::GainedFocus)
		lostFocus = false;
	if(lostFocus)
		return ;
	sf::Vector2i ms = sf::Mouse::getPosition(*wndPtr);
	sf::RectangleShape shp(sf::Vector2f(1,1));
	shp.setOrigin(0.5f,0.5f);
	shp.setPosition(ms.x,ms.y);
	TB.setOutlineColor(sf::Color::Black);
	
	if(text.getGlobalBounds().width >= TB.getSize().x-10)
	{
		NOCIT++;
		first = false;
	}
	
	if(TB.getGlobalBounds().intersects(shp.getGlobalBounds()))
	{
		SetCursor(LoadCursor(NULL, IDC_IBEAM));
		TB.setOutlineColor(sf::Color(0,128,255));
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && leftPress)
		{
			focus = true;
			leftPress = false;
		}
		else
			leftPress = true;
	}
	else
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && leftPress)
		{
			focus = false;
			leftPress = false;
		}
		else
			leftPress = true;
	}
	if(focus)
	{
		if(e.type == sf::Event::TextEntered)
		{
			bool TE = false;
			if(!onlyNumbers)
			{
				if((e.text.unicode == 8) || (e.text.unicode > 31 && e.text.unicode < 128))
					TE = true;
			}
			else
			{
				if((e.text.unicode == 8) || (e.text.unicode >= 48 && e.text.unicode <= 57))
					TE = true;
			}
			if(TE)
			{
				if(e.text.unicode == 8)
				{
					if(yedek.length() > 0)
					{
						yedek.resize(yedek.size()-1);
						if(NOCIT != 0)
						NOCIT--;
					}
				}
				else if(e.text.unicode == 13)
					focus = false;
				else
				{
					if(maxChar != 0)
					{
						if(maxChar != yedek.length())
							yedek.push_back(static_cast<char>(e.text.unicode));
					}
					else
						yedek.push_back(static_cast<char>(e.text.unicode));
				}
			}
		}
	}
	if(!focus)
	{
		if(text.getString() == "")
			drawHint = true;
	}
	if(focus)
		drawHint = false;
	if(!first)
	{
		string abc;
		for(int i = NOCIT; i < yedek.length(); i++)
			abc.push_back(yedek[i]);
		text.setString(abc);
	}
	else
		text.setString(yedek);
}
bool Form::TextBox::getFocus()
{
	return focus;
}
bool Form::TextBox::isEmpty()
{
	return yedek.empty();
}
void Form::TextBox::OnlyNumbers(bool ON)
{
	onlyNumbers = ON;
	yedek.clear();
	text.setString(yedek);
	first = true;
}
bool Form::TextBox::getOnlyNumbers()
{
	return onlyNumbers;
}
