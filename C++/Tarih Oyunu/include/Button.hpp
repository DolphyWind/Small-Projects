#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "UIElement.hpp"
#include "general.hpp"

enum class EventMode
{
    COLOR = 0,
    TEXTURE = 1,
    COLOR_WITH_TEXTURE = 2
};

class Button : public UIElement
{
    sf::Text text;
    sf::RectangleShape shape;

    sf::Color normalColor;
    sf::Color outlineColor;
    sf::Color hoverColor;
    sf::Color disabledColor;
    sf::Color textColor;
    sf::Color pressedColor;

    sf::Texture *normalTexture;
    sf::Texture *hoverTexture;
    sf::Texture *disabledTexture;
    sf::Texture *pressedTexture;

    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f textOffset;
    unsigned int thickness = 0;
    unsigned int characterSize = 20;
    sf::Font *font;
    bool disabled = false;
    bool hovering = false;
    bool pressing = false;
    EventMode mode;
    TextAlignment textAlignment = TextAlignment::CENTER;

    void createDefaultButton();
public:
    Button();
    Button(sf::RenderWindow *window);
    Button(sf::RenderWindow *window, sf::Vector2f size);
    Button(sf::RenderWindow *window, sf::Font *font, sf::Vector2f size, unsigned int characterSize, sf::String str);

    void setString(sf::String str);
    sf::String getString();
    void setSize(sf::Vector2f size);
    sf::Vector2f getSize();
    void setPosition(sf::Vector2f pos);
    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    void setTextOffset(sf::Vector2f offset);
    void setTextOffset(float x, float y);
    sf::Vector2f getTextOffset();

    void setFont(sf::Font *font);
    sf::Font *getFont();
    void setCharacterSize(unsigned int characterSize);
    unsigned int getCharacterSize();

    void setEventMode(EventMode mode);
    EventMode getEventMode();
    void setTextAlignment(TextAlignment alignment);
    TextAlignment getTextAlignment();

    void setNormalColor(sf::Color color);
    sf::Color getNormalColor();
    void setOutlineColor(sf::Color color);
    sf::Color getOutlineColor();
    void setHoverColor(sf::Color color);
    sf::Color getHoverColor();
    void setDisabledColor(sf::Color color);
    sf::Color getDisabledColor();
    void setTextColor(sf::Color color);
    sf::Color getTextColor();
    void setPressedColor(sf::Color color);
    sf::Color getPressedColor();

    void setNormalTexture(sf::Texture *texture);
    sf::Texture *getNormalTexture();
    void setHoverTexture(sf::Texture *texture);
    sf::Texture *getHoverTexture();
    void setPressedTexture(sf::Texture *texture);
    sf::Texture *getPressedTexture();
    void setDisabledTexture(sf::Texture *texture);
    sf::Texture *getDisabledTexture();

    void setOutlineThickness(unsigned int thickness);
    unsigned int getOutlineThickness();
    void setDisabled(bool disabled);
    bool getDisabled();

    bool isClicked();
    bool isHovering();

    void draw() override;
    void update() override;
    ~Button() {};
};