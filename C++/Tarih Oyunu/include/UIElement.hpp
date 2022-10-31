#pragma once
#include <SFML/Graphics.hpp>

enum class TextAlignment
{
    TOPLEFT = 0,
    TOP = 1,
    TOPRIGHT = 2,
    LEFT = 3,
    CENTER = 4,
    RIGHT = 5,
    BOTTOMLEFT = 6,
    BOTTOM = 7,
    BOTTOMRIGHT = 8,
    COUNT
};

class UIElement
{
protected:
    sf::RenderWindow *window;
public:
    UIElement();
    ~UIElement();

    void setWindow(sf::RenderWindow *window);
    sf::RenderWindow *getWindow();
    virtual void draw() = 0;
    virtual void update() = 0;
};