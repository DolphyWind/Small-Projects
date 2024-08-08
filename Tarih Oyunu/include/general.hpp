#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

sf::Color mixColors(std::vector<sf::Color> colors);
sf::Color colorGradiant(sf::Color c1, sf::Color c2, float distance);