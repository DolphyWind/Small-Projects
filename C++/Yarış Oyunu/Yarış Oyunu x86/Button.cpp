#include "Button.h"

void addStr(string &mainStr, unsigned int after, string afterstr)
{
	string newstr;
	for(int i = 0; i < mainStr.length(); i++)
	{
		newstr.push_back(mainStr[i]);
		if(i == after)
			newstr+=afterstr;
	}
	mainStr = newstr;
}

string toThere(string str, unsigned int there)
{
	string newstr;
	for(int i = 0; i < there; i++)
	{
		newstr.push_back(str[i]);
	}
	return newstr;
}
template <typename T>
string toString(T arg)
{
	stringstream ss;
	ss << arg;
	return ss.str();
}
void Control(sf::Text &t, sf::RectangleShape rect, int x, unsigned int &c)
{
	string txtstr = t.getString();
	const sf::Font* f2 = t.getFont();
	sf::Font f = *f2;
	if(t.getGlobalBounds().width >= rect.getGlobalBounds().width-x)
	{
		for(int i = 0; i < t.getString().toAnsiString().length(); i++)
		{
			sf::Text T2(toThere(txtstr,i),f);
			if(T2.getGlobalBounds().width >= rect.getGlobalBounds().width-x && i != 0)
			{
				c = i;
				break;
			}
		}
		int len = txtstr.length();
		for(int i = c; i < len; i+=c)
		{
			addStr(txtstr,i,"\n");
		}
		t.setString(txtstr);
		setToCenter(t,rect);
	}
	else
		return ;
}
void setToCenter(sf::Text &t, sf::RectangleShape rect)
{
	t.setPosition(sf::Vector2f(rect.getPosition().x+rect.getGlobalBounds().width/2-t.getGlobalBounds().width/2,rect.getPosition().y+rect.getGlobalBounds().height/2-t.getGlobalBounds().height/1.5));
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

Form::Button::Button()
{
	clicked = false;
	button.setOutlineColor(sf::Color(0,0,0));
	button.setOutlineThickness(1);
	setToCenter(text,button);
	text.setColor(sf::Color(0,0,0));
	character = 0;
	cSize = 30;
	text.setCharacterSize(cSize);
	button.setFillColor(sf::Color(17, 159, 191));
	c = sf::Color(17, 159, 191);
	lostFocus = false;
	setToCenter(text,button);
}
Form::Button::Button(sf::Vector2f size, sf::Font &f, string txt, unsigned int charSize)
{
	clicked = false;
	Size = size;
	character = 0;
	button.setSize(size);
	button.setFillColor(sf::Color(17, 159, 191));
	c = sf::Color(17, 159, 191);
	text.setFont(f);
	text.setString(txt);
	text.setCharacterSize(charSize);
	cSize = charSize;
	Control(text,button,10,character);
	button.setOutlineColor(sf::Color(0,0,0));
	button.setOutlineThickness(1);
	text.setColor(sf::Color::Black);
	lostFocus = false;
	setToCenter(text,button);
}
void Form::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(button, states);
	target.draw(text, states);
}
void Form::Button::setPosition(sf::Vector2f pos)
{
	position = pos;
	button.setPosition(pos);
	setToCenter(text,button);
}
void Form::Button::setPosition(float posX, float posY)
{
	position = sf::Vector2f(posX,posY);
	button.setPosition(position);
	setToCenter(text,button);
}
sf::Vector2f Form::Button::getPosition()
{
	return position;
}
sf::Vector2f Form::Button::getSize()
{
	return Size;
}
void Form::Button::setSize(sf::Vector2f size)
{
	Size = size;
	button.setSize(size);
	setToCenter(text,button);
}
void Form::Button::setSize(float sizeX, float sizeY)
{
	Size = sf::Vector2f(sizeX, sizeY);
	button.setSize(Size);
	setToCenter(text,button);
}
void Form::Button::setText(sf::Text txt)
{
	text.setString(txt.getString());
	Control(text,button,10,character);
	setToCenter(text,button);
}
void Form::Button::setString(string txt)
{
	text.setString(txt);
	Control(text,button,10,character);
	setToCenter(text,button);
}
bool Form::Button::isLeftClicked(sf::RenderWindow& window, sf::Event e)
{
	if(e.type == sf::Event::LostFocus) lostFocus = true;
	if(e.type == sf::Event::GainedFocus) lostFocus = false;
	if(lostFocus) return false;
	
	sf::RectangleShape MS(sf::Vector2f(1,1));
	MS.setOrigin(0.5,0.5);
	MS.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
	if(button.getGlobalBounds().intersects(MS.getGlobalBounds()))
	{
		button.setFillColor(sf::Color(c.r,c.g,c.b,c.a-55));
		SetCursor(LoadCursor(NULL, IDC_HAND));
	}
	else
	{
		button.setFillColor(sf::Color(c.r,c.g,c.b,c.a));
	}
	return (sf::Mouse::isButtonPressed(sf::Mouse::Left) && button.getGlobalBounds().intersects(MS.getGlobalBounds()));
}
bool Form::Button::isRightClicked(sf::RenderWindow& window, sf::Event e)
{
	if(e.type == sf::Event::LostFocus) lostFocus = true;
	if(e.type == sf::Event::GainedFocus) lostFocus = false;
	if(lostFocus) return false;
	
	sf::RectangleShape MS(sf::Vector2f(1,1));
	MS.setOrigin(0.5,0.5);
	MS.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
	if(button.getGlobalBounds().intersects(MS.getGlobalBounds()))
	{
		button.setFillColor(sf::Color(c.r,c.g,c.b,c.a-55));
		SetCursor(LoadCursor(NULL, IDC_HAND));
	}
	else
	{
		button.setFillColor(sf::Color(c.r,c.g,c.b,c.a));
	}
	return (sf::Mouse::isButtonPressed(sf::Mouse::Right) && button.getGlobalBounds().intersects(MS.getGlobalBounds()));
}
sf::FloatRect Form::Button::getGlobalBounds()
{
	return button.getGlobalBounds();
}
sf::FloatRect Form::Button::getLocalBounds()
{
	return button.getLocalBounds();
}
sf::FloatRect Form::Button::getTextGlobalBounds()
{
	return text.getGlobalBounds();
}
sf::FloatRect Form::Button::getTextLocalBounds()
{
	return text.getGlobalBounds();
}
unsigned int Form::Button::getCharacterSize()
{
	return cSize;
}
void Form::Button::setCharacterSize(unsigned int CS)
{
	cSize = CS;
	text.setCharacterSize(CS);
}
void Form::Button::setFont(sf::Font &f)
{
	text.setFont(f);
}
const sf::Font *Form::Button::getFont()
{
	return text.getFont();
}
void Form::Button::setButtonColor(sf::Color col)
{
	button.setFillColor(col);
	c = col;
}
sf::Color Form::Button::getButtonColor()
{
	return button.getFillColor();
}
void Form::Button::setTextColor(sf::Color col)
{
	text.setColor(col);
}
sf::Color Form::Button::getTextColor()
{
	return text.getColor();
}
