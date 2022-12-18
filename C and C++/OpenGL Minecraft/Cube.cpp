#include "Cube.h"
#include <GL/glut.h>
#include <SFML/Graphics.hpp>
#include <iostream>

Cube::Cube()
{
	size = 0.0f;
	position.x = 0.0f, position.y = 0.0f, position.z = 0.0f;
}
void Cube::setSize(float new_size)
{
	size = new_size;
}
void Cube::setPosition(float x, float y, float z)
{
	position = sf::Vector3f(x, y, z);
}
void Cube::setPosition(sf::Vector3f new_position)
{
	position = new_position;
}
void Cube::setTopImage(sf::Image image)
{
	topImage = image;
}
void Cube::setBottomImage(sf::Image image)
{
	bottomImage = image;
}
void Cube::setSideImages(size_t index, sf::Image image)
{
	sideImages[index] = image;
}
float Cube::getSize()
{
	return size;
}
sf::Image Cube::getBottomImage()
{
	return bottomImage;
}
sf::Image Cube::getTopImage()
{
	return topImage;
}
sf::Image Cube::getSideImage(size_t index)
{
	return sideImages[index];
}
sf::Vector3f Cube::getPosition()
{
	return position;
}
void Cube::
