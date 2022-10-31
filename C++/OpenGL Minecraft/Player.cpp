#include "Player.h"

double DegtoRad(double val)
{
	const double PI = 3.14159265358979323846264338327950288;
	return val * PI / 180;
}

double RadtoDeg(double val)
{
	const double PI = 3.14159265358979323846264338327950288;
	return val * 180 / PI;
}

bool checkWorld(int x, int y, int z, bool world[1000][60][1000])
{
	if(x < 0 || x >= 1000 || y < 0 || y >= 60 || z < 0 || z >= 1000) return false;
	return world[x][y][z];
}

Player::Player()
{
	position.x = 0.0f, position.y = 1.0f, position.z = 0.0f;
	lx=0.0f,lz=-1.0f, ly = 0.0f, _lx = 0.0f, _lz = -1.0f;
	angle.x = 0.0f, ang_90 = 90 * PI / 180.0f, angle.y = 0.0f;
	speed = 1.0f, LRSpeed = 1.0f;
	frametime = 1.0f;
	onGround = false;jumping = false;
}
void Player::setPosition(float dx, float dy, float dz)
{
	position.x = dx;
	position.y = dy;
	position.z = dz;
}

void Player::calculateAngles(float deltaAngle, float deltaAngleY)
{
	angle.x += deltaAngle;
	angle.y += deltaAngleY;
	if(angle.y < DegtoRad(-89.99f)) angle.y = DegtoRad(-89.99f);
	if(angle.y > DegtoRad(89.99f)) angle.y = DegtoRad(89.99f);
	angle.x = fmod(angle.x, DegtoRad(360.0f));
	ang_90 = angle.x + (90 * PI / 180);
	lx = sin(angle.x);
	ly = tan(angle.y);
	lz = -cos(angle.x);
	_lx = sin(ang_90);
	_lz = -cos(ang_90);
}
void Player::move(float mX, float mY, float mZ)
{
	position.x += mX;
	position.y += mY;
	position.z += mZ;
}
void Player::goForward(bool world[1000][60][1000])
{
	position.x += speed * frametime * lx;
	position.z += speed * frametime * lz;
	if(position.x >= 0 && position.x < 1000 && position.z >= 0 && position.z < 1000)
	{
		if(checkCollision(world))
		{
			position.x -= speed * frametime * lx;
			position.z -= speed * frametime * lz;
		}
	}
}
void Player::goBackward()
{
	position.x += -speed * frametime * lx;
	position.z += -speed * frametime * lz;
}
void Player::goLeft()
{
	position.x += -LRSpeed * frametime * _lx;
	position.z += -LRSpeed * frametime * _lz;
}
void Player::goRight()
{
	position.x += LRSpeed * frametime * _lx;
	position.z += LRSpeed * frametime * _lz;
}
void Player::look()
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(position.x, position.y, position.z,
              position.x+lx, position.y+ly,  position.z+lz,
              0.0f, 1.0f,  0.0f);
}
void Player::setFrametime(sf::Time t)
{
	frametime = t.asSeconds();
}
sf::Vector3f Player::getPosition()
{
	return position;
}
void Player::setSpeed(float newSpeed)
{
	speed = newSpeed;
}
void Player::setLRSpeed(float newLRSpeed)
{
	LRSpeed = newLRSpeed;
}
sf::Vector2f Player::getAngles()
{
	return angle;
}
sf::Vector3f Player::getL()
{
	return sf::Vector3f(lx, ly, lz);
}
bool Player::checkCollision(bool world[1000][60][1000])
{
	return false;
}
void Player::checkOnGround(bool world[1000][60][1000])
{
	onGround = (checkWorld(position.x, floor(position.y - 1), position.z, world));
}
bool Player::getOnGround()
{
	return onGround;
}
void Player::updateY()
{
	if(jumping)
	{
		position.y += jumpSpeed * frametime;
		jumpSpeed -= deltaJump;
		jumpDivide -= 2.5f;
		deltaJump += deltaJump / jumpDivide;
		
		if(position.y >= baseY + jumpHeight) 
		{
			position.y = baseY + jumpHeight;
			jumpSpeed = jumpSpeedDefault;
			jumpDivide = 100.0f;
			deltaJump = deltaJumpDefault;
			jumping = false;
			return;
		}
	}
	if(!onGround && !jumping)
	{
		position.y -= jumpSpeed * frametime;
		jumpSpeed += deltaJump;
		if(onGround)
		{
			position.y = int(position.y) + 0.84;
			jumpSpeed = jumpSpeedDefault;
			deltaJump = deltaJumpDefault;
			jumpDivide = 100;
			return;
		}
	}
	if(!jumping && onGround)
	{
		position.y = int(position.y) + 0.84f;
	}
}
void Player::jump()
{
	if(onGround && !jumping)
	{
		baseY = position.y;
		jumping = true;
	}
}
