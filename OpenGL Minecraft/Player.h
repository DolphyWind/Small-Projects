#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <cmath>
#include <math.h>
#include <GL/glut.h>
#include <SFML/Graphics.hpp>
using namespace std;

class Player
{
	private:
		sf::Vector3f position;
		sf::Vector2f angle;
		double lx, ly, lz, _lx, _lz;
		double ang_90;
		float speed, LRSpeed;
		const double PI = 3.14159265358979323846264338327950288;
		float frametime;
		float size_2 = 0.25f;
		bool onGround, jumping;
		float jumpSpeedDefault = 5;
		float jumpSpeed = 5;
		float deltaJumpDefault = 0.050;
		float deltaJump = 0.050;
		float baseY;
		float jumpHeight = 2.0f;
		float jumpDivide = 100.0f;
	public:
		Player();
		void setPosition(float dx, float dy, float dz);
		void calculateAngles(float deltaAngle, float deltaAngleY);
		void move(float mX, float mY, float mZ);
		void goForward(bool world[1000][60][1000]);
		void goBackward();
		void goLeft();
		void goRight();
		void look();
		void setFrametime(sf::Time t);
		sf::Vector3f getPosition();
		void setSpeed(float newSpeed);
		void setLRSpeed(float newLRSpeed);
		sf::Vector2f getAngles();
		sf::Vector3f getL();
		bool checkCollision(bool world[1000][60][1000]);
		void checkOnGround(bool world[1000][60][1000]);
		bool getOnGround();
		void updateY();
		void jump();
};

#endif
