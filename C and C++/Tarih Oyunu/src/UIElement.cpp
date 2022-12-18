#include "UIElement.hpp"

UIElement::UIElement()
{

}

UIElement::~UIElement()
{

}

void UIElement::setWindow(sf::RenderWindow *window) 
{
    this->window = window;
}

sf::RenderWindow* UIElement::getWindow() 
{
    return this->window;
}