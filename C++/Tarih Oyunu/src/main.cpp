#include <iostream>
#include "MainWindow.hpp"

int main()
{
    MainWindow window(sf::Vector2u(626, 720), "60 Saniyede Tarih", sf::Style::Close);
    window.mainLoop();
    
    return 0;
}