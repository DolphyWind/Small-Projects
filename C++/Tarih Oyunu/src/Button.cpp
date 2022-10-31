#include "Button.hpp"

Button::Button() 
{
    this->createDefaultButton();    
}

Button::Button(sf::RenderWindow *window) 
{
    this->createDefaultButton();
    this->setWindow(window);
}

Button::Button(sf::RenderWindow *window, sf::Vector2f size) 
{
    this->createDefaultButton();
    this->setWindow(window);
    this->setSize(size);
    this->setPosition(position);
}

Button::Button(sf::RenderWindow *window, sf::Font *font, sf::Vector2f size, unsigned int characterSize, sf::String str) 
{
    this->createDefaultButton();
    this->setWindow(window);
    this->setSize(size);
    this->setFont(font);
    this->setCharacterSize(characterSize);
    this->setString(str);
}

void Button::setString(sf::String str) 
{
    this->text.setString(str);
    this->setPosition(position);
}

sf::String Button::getString() 
{
    return this->text.getString();
}

void Button::setSize(sf::Vector2f size) 
{
    this->size = size;
    this->shape.setSize(size);
    this->setPosition(position);
}

sf::Vector2f Button::getSize() 
{
    return this->size;
}

void Button::setPosition(sf::Vector2f pos) 
{
    this->position = pos;
    this->shape.setPosition(pos);
    sf::Vector2f shapePos = this->shape.getPosition();
    sf::Vector2f shapeSize = this->shape.getSize();
    sf::Vector2f textSize;
    textSize.x = this->text.getLocalBounds().width;
    textSize.y = this->text.getLocalBounds().height;

    sf::Vector2f alignment;
    alignment.x = float((int)textAlignment % 3) / 2;
    alignment.y = float((int)textAlignment / 3) / 2;
    this->text.setOrigin(sf::Vector2f(
        textSize.x * alignment.x - this->text.getLocalBounds().left,
        textSize.y * alignment.y
    ));
    this->text.setPosition(
        shapePos.x + (shapeSize.x * alignment.x) + textOffset.x,
        shapePos.y + (shapeSize.y * alignment.y) + textOffset.y
    );
}

void Button::setPosition(float x, float y) 
{
    this->setPosition(sf::Vector2f(x, y));
}

sf::Vector2f Button::getPosition() 
{
    return this->position;
}

void Button::setTextOffset(sf::Vector2f offset) 
{
    this->textOffset = offset;
    this->setPosition(this->position);
}

void Button::setTextOffset(float x, float y) 
{
    this->setTextOffset(sf::Vector2f(x, y));
}

sf::Vector2f Button::getTextOffset() 
{
    return this->textOffset;
}

void Button::setFont(sf::Font *font) 
{
    this->font = font;
    this->text.setFont(*font);
    this->setPosition(position);
}

sf::Font* Button::getFont() 
{
    return this->font;
}

void Button::setCharacterSize(unsigned int characterSize) 
{
    this->text.setCharacterSize(characterSize);
    this->characterSize = characterSize;
}

unsigned int Button::getCharacterSize() 
{
    return this->characterSize;
}

void Button::setEventMode(EventMode mode) 
{
    switch (mode)
    {
    case EventMode::COLOR:
        this->shape.setFillColor(this->normalColor);
        this->shape.setTexture(NULL, true);
        break;
    case EventMode::TEXTURE:
        this->shape.setFillColor(sf::Color::White);
        this->shape.setTexture(normalTexture, true);
        break;
    case EventMode::COLOR_WITH_TEXTURE:
        this->shape.setFillColor(this->normalColor);
        this->shape.setTexture(this->normalTexture, true);
        break;
    
    default:
        break;
    }
    this->mode = mode;
}

EventMode Button::getEventMode() 
{
    return this->mode;
}

void Button::setTextAlignment(TextAlignment alignment) 
{
    this->textAlignment = alignment;
    this->setPosition(this->position);
}

TextAlignment Button::getTextAlignment() 
{
    return this->textAlignment;
}

void Button::update() 
{
    if(disabled || !this->window->hasFocus()) return;
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::Vector2f ms = window->mapPixelToCoords(mousePos);
    sf::Vector2f sp = shape.getPosition();

    if(ms.x >= sp.x && ms.y >= sp.y && ms.x <= sp.x + size.x && ms.y <= sp.y + size.y)
    {
        if(!hovering)
        {
            if(!pressing)
            {
                if(this->shape.getTexture() != this->hoverTexture && this->hoverTexture != NULL && this->mode == EventMode::TEXTURE)
                    this->shape.setTexture(this->hoverTexture, true);
                if(this->mode != EventMode::TEXTURE)
                {
                    this->shape.setFillColor(hoverColor);
                    this->text.setFillColor(mixColors({
                        this->textColor,
                        this->hoverColor
                    }));
                }
            }
            this->hovering = true;
        }
    }
    else 
    {
        if(this->mode != EventMode::TEXTURE)
        {
            this->shape.setFillColor(normalColor);
            this->text.setFillColor(this->textColor);
        }
        if(this->shape.getTexture() != this->normalTexture && this->normalTexture != NULL&& this->mode != EventMode::COLOR)
            this->shape.setTexture(this->normalTexture, true);
        this->hovering = false;
    }
}

void Button::setNormalColor(sf::Color color) 
{
    this->normalColor = color;
    this->shape.setFillColor(color);
}

sf::Color Button::getNormalColor() 
{
    return normalColor;
}

void Button::setOutlineColor(sf::Color color) 
{
    this->outlineColor = color;
    this->shape.setOutlineColor(color);
}

sf::Color Button::getOutlineColor() 
{
    return this->outlineColor;
}

void Button::setHoverColor(sf::Color color) 
{
    this->hoverColor = color;
}

sf::Color Button::getHoverColor() 
{
    return this->hoverColor;
}

void Button::setDisabledColor(sf::Color color) 
{
    this->disabledColor = color;
}

sf::Color Button::getDisabledColor() 
{
    return this->disabledColor;
}

void Button::setTextColor(sf::Color color) 
{
    this->textColor = color;
    this->text.setFillColor(color);
}

sf::Color Button::getTextColor() 
{
    return this->textColor;
}

void Button::setPressedColor(sf::Color color) 
{
    this->pressedColor = color;
}

sf::Color Button::getPressedColor() 
{
    return this->pressedColor;
}

void Button::setOutlineThickness(unsigned int thickness) 
{
    this->thickness = thickness;
    this->shape.setOutlineThickness(thickness);
}

unsigned int Button::getOutlineThickness() 
{
    return this->thickness;
}

void Button::setNormalTexture(sf::Texture *texture) 
{
    this->normalTexture = texture;
    if(this->mode != EventMode::COLOR)
        this->shape.setTexture(texture, true);
}

sf::Texture* Button::getNormalTexture() 
{
    return this->normalTexture;
}

void Button::setHoverTexture(sf::Texture *texture) 
{
    this->hoverTexture = texture;
}

sf::Texture* Button::getHoverTexture() 
{
    return this->hoverTexture;
}

void Button::setPressedTexture(sf::Texture *texture) 
{
    this->pressedTexture = texture;
}

sf::Texture* Button::getPressedTexture() 
{
    return this->pressedTexture;
}

void Button::setDisabledTexture(sf::Texture *texture) 
{
    this->disabledTexture = texture;
}

sf::Texture* Button::getDisabledTexture() 
{
    return this->disabledTexture;
}

void Button::setDisabled(bool disabled) 
{
    this->disabled = disabled;
    if(disabled)
    {
        if(this->mode != EventMode::TEXTURE)
        {
            this->shape.setFillColor(this->disabledColor);
            this->text.setFillColor(mixColors({
                this->textColor,
                this->disabledColor
            }));
        }
        if(this->shape.getTexture() != this->disabledTexture && this->disabledTexture != NULL && this->mode == EventMode::TEXTURE)
            this->shape.setTexture(this->disabledTexture, true);
        this->hovering = false;
        this->pressing = false;
    }
    else
    {
        if(this->mode != EventMode::TEXTURE)
        {
            this->shape.setFillColor(this->normalColor);
            this->text.setFillColor(mixColors({
                this->textColor,
                this->normalColor
            }));
        }
        if(this->shape.getTexture() != this->normalTexture && this->normalTexture != NULL && this->mode != EventMode::COLOR)
            this->shape.setTexture(this->normalTexture, true);
    }
}

bool Button::getDisabled() 
{
    return this->disabled;
}

bool Button::isClicked() 
{
    if(!window->hasFocus() || !this->hovering) return false;
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        pressing = true;
        if(this->mode != EventMode::TEXTURE)
        {
            this->shape.setFillColor(pressedColor);
            this->text.setFillColor(mixColors({
                this->textColor,
                this->pressedColor
            }));
        }
        if(this->shape.getTexture() != this->pressedTexture && this->pressedTexture != NULL && this->mode == EventMode::TEXTURE)
            this->shape.setTexture(this->pressedTexture, true);
        return true;
    }
    else
    {
        if(this->mode != EventMode::TEXTURE && this->hovering)
        {
            this->shape.setFillColor(hoverColor);
            this->text.setFillColor(mixColors({
                this->textColor,
                this->hoverColor
            }));
        }
        if(this->shape.getTexture() != this->hoverTexture && this->hoverTexture != NULL && this->mode == EventMode::TEXTURE && this->hovering)
            this->shape.setTexture(this->hoverTexture, true);
        pressing = false;
    }
    return false;
}

bool Button::isHovering() 
{
    return this->hovering;
}

void Button::draw() 
{
    this->window->draw(this->shape);
    this->window->draw(this->text);
}

void Button::createDefaultButton() 
{
    this->setWindow(NULL);
    this->setEventMode(EventMode::COLOR);
    this->setCharacterSize(20);
    this->setDisabled(false);   
    this->setNormalColor(sf::Color(65, 105, 225));
    this->setHoverColor(sf::Color(0, 191, 255));
    this->setPressedColor(sf::Color(65, 105, 225));
    this->setDisabledColor(sf::Color(128, 128, 128));
    this->setTextColor(sf::Color(0, 0, 0));
    this->setOutlineColor(sf::Color(0, 0, 0));
    this->setFont(NULL);
    this->setPosition(sf::Vector2f(0, 0));
    this->setOutlineThickness(0);
    this->setString("");
    this->setNormalTexture(NULL);
    this->setPressedTexture(NULL);
    this->setDisabledTexture(NULL);
    this->setHoverTexture(NULL);
}
