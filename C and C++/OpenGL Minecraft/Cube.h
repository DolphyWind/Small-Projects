#ifndef CUBE_H
#define CUBE_H
#include <GL/glut.h>
#include <SFML/Graphics.hpp>
#include <iostream>

class Cube
{
	private:
		float size;
		sf::Vector3f position;
		sf::Image topImage;
		sf::Image sideImages[4];
		sf::Image bottomImage;
		GLuint textures[6];
	public:
		Cube();
		void setTopImage(sf::Image image);
		void setSideImages(size_t index, sf::Image image);
		void setBottomImage(sf::Image image);
		void setSize(float new_size);
		void setPosition(float x, float y, float z);
		void setPosition(sf::Vector3f new_position);
		float getSize();
		sf::Vector3f getPosition();
		sf::Image getTopImage();
		sf::Image getSideImage(size_t index);
		sf::Image getBottomImage();
		void generateTextures();
		void draw();
};

#endif
