#include <GL/glut.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Player.h"
using namespace std;

const float PI = 3.14159265358979323846;
const float FPS = 60.0f;

double Deg2Rad(double deg)
{
	return deg * PI / 180;
}

double Rad2Deg(double rad)
{
	return rad * 180 / PI;
}

bool checkW(int x, int y, int z, bool world[1000][60][1000])
{
	if(x < 0 || x >= 1000 || y < 0 || y >= 60 || z < 0 || z >= 1000) return false;
	return world[x][y][z];
}

template<typename T>
string toString(T arg)
{
	stringstream ss;
	ss << arg;
	return ss.str();
}

bool world[1000][60][1000];

sf::Vector2f screen_size(1024, 768);
//Always In Center
sf::Vector2f screen_pos(sf::VideoMode::getDesktopMode().width / 2 - screen_size.x / 2, sf::VideoMode::getDesktopMode().height / 2 - screen_size.y / 2);

Player p;
HWND hwnd;

void display();
void keyboard(unsigned char key, int x, int y);
void timerFunc(int val);
void idle();
void resize(int w, int h);

sf::Image cursor_i;
sf::Image grass_top, grass_side, grass_bottom;
sf::Image skybox_top, skybox_side[4], skybox_bottom;
int fps = 0, last_fps = 0;
sf::Clock c;
sf::Clock fps_c;
sf::Time fps_t;
float dist = 20;
bool lPress = true, rPress = true;
sf::Image chan_i;

GLuint textures[6];
GLuint skybox[6];
GLuint chan;

void generateTex(GLuint &t, sf::Image i)
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures( 1, &t ); // top
	glBindTexture( GL_TEXTURE_2D, t );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, i.getSize().x, i.getSize().y,GL_RGBA, GL_UNSIGNED_BYTE, i.getPixelsPtr() );
	glDisable(GL_TEXTURE_2D);
}

void jackie(float x, float y, float z, float size, float rotation[4])
{
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, chan);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRotatef(rotation[0], rotation[1], rotation[2], rotation[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(x, y, z);
		glTexCoord2f(1, 0); glVertex3f(x + size, y, z);
		glTexCoord2f(1, 1); glVertex3f(x + size, y - size, z);
		glTexCoord2f(0, 1); glVertex3f(x, y - size, z);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

void generateTextures()
{
	generateTex(textures[0], grass_top);
	for(int i = 1; i < 5; i++) generateTex(textures[i], grass_side);
	generateTex(textures[5], grass_bottom);
	
	generateTex(skybox[0], skybox_top);
	for(int i = 0; i < 4; i++)
		generateTex(skybox[i + 1], skybox_side[i]);
	generateTex(skybox[5], skybox_bottom);
}

void drawCube(float x, float y, float z, float size)
{
	glEnable(GL_TEXTURE_2D);
	
	if(checkW(x, y + 1, z, world)) glBindTexture( GL_TEXTURE_2D, textures[5]);
	else glBindTexture( GL_TEXTURE_2D, textures[0] );
	
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);glVertex3f(x, y, z);
		glTexCoord2f(1, 0);glVertex3f(x+size, y, z);
		glTexCoord2f(1, 1);glVertex3f(x+size, y, z+size);
		glTexCoord2f(0, 1);glVertex3f(x, y, z+size);
	glEnd();
	
	if(checkW(x, y + 1, z, world)) glBindTexture( GL_TEXTURE_2D, textures[5]);
	else glBindTexture( GL_TEXTURE_2D, textures[1] );
	
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);glVertex3f(x, y, z);
		glTexCoord2f(1, 0);glVertex3f(x + size, y, z);
		glTexCoord2f(1, 1);glVertex3f(x + size, y - size, z);
		glTexCoord2f(0, 1);glVertex3f(x, y - size, z);
	glEnd();
	
	if(checkW(x, y + 1, z, world)) glBindTexture( GL_TEXTURE_2D, textures[5]);
	else glBindTexture( GL_TEXTURE_2D, textures[2] );
	
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);glVertex3f(x, y, z);
		glTexCoord2f(1, 0);glVertex3f(x, y, z + size);
		glTexCoord2f(1, 1);glVertex3f(x, y - size, z + size);
		glTexCoord2f(0, 1);glVertex3f(x, y - size, z);
	glEnd();
	
	if(checkW(x, y + 1, z, world)) glBindTexture( GL_TEXTURE_2D, textures[5]);
	else glBindTexture( GL_TEXTURE_2D, textures[3] );
	
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);glVertex3f(x, y, z + size);
		glTexCoord2f(1, 0);glVertex3f(x + size, y, z + size);
		glTexCoord2f(1, 1);glVertex3f(x + size, y - size, z + size);
		glTexCoord2f(0, 1);glVertex3f(x, y - size, z + size);
	glEnd();
	
	if(checkW(x, y + 1, z, world)) glBindTexture( GL_TEXTURE_2D, textures[5]);
	else glBindTexture( GL_TEXTURE_2D, textures[4] );
	
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);glVertex3f(x + size, y, z);
		glTexCoord2f(1, 0);glVertex3f(x + size, y, z + size);
		glTexCoord2f(1, 1);glVertex3f(x + size, y - size, z + size);
		glTexCoord2f(0, 1);glVertex3f(x + size, y - size, z);
	glEnd();
	
	glBindTexture( GL_TEXTURE_2D, textures[5] );
	
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);glVertex3f(x, y - size, z);
		glTexCoord2f(1, 0);glVertex3f(x+size, y - size, z);
		glTexCoord2f(1, 1);glVertex3f(x+size, y - size, z+size);
		glTexCoord2f(0, 1);glVertex3f(x, y - size, z+size);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

void drawSkybox()
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, skybox[0]); // top 
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);glVertex3f(-1000.0f, 500.0f, -1000.0f);
		glTexCoord2f(1, 0);glVertex3f(2000.0f, 500.0f, -1000.0f);
		glTexCoord2f(1, 1);glVertex3f(2000.0f, 500.0f, 2000.0f);
		glTexCoord2f(0, 1);glVertex3f(-1000.0f, 500.0f, 2000.0f);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, skybox[3]); // left
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(-1000.0f, 500.0f, -1000.0f);
		glTexCoord2f(0, 1);glVertex3f(-1000.0f, 500.0f, 2000.0f);
		glTexCoord2f(0, 0);glVertex3f(-1000.0f, -500.0f, 2000.0f);
		glTexCoord2f(1, 0);glVertex3f(-1000.0f, -500.0f, -1000.0f);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, skybox[1]); // right
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);glVertex3f(2000.0f, 500.0f, -1000.0f);
		glTexCoord2f(1, 0);glVertex3f(2000.0f, 500.0f, 2000.0f);
		glTexCoord2f(1, 1);glVertex3f(2000.0f, -500.0f, 2000.0f);
		glTexCoord2f(0, 1);glVertex3f(2000.0f, -500.0f, -1000.0f);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, skybox[4]); // back
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);glVertex3f(-1000.0f, 500.0f, -1000.0f);
		glTexCoord2f(1, 0);glVertex3f(2000.0f, 500.0f, -1000.0f);
		glTexCoord2f(1, 1);glVertex3f(2000.0f, -500.0f, -1000.0f);
		glTexCoord2f(0, 1);glVertex3f(-1000.0f, -500.0f, -1000.0f);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, skybox[2]); // front
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(-1000.0f, 500.0f, 2000.0f);
		glTexCoord2f(0, 1);glVertex3f(2000.0f, 500.0f, 2000.0f);
		glTexCoord2f(0, 0);glVertex3f(2000.0f, -500.0f, 2000.0f);
		glTexCoord2f(1, 0);glVertex3f(-1000.0f, -500.0f, 2000.0f);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, skybox[5]); // bottom 
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);glVertex3f(-1000.0f, -500.0f, -1000.0f);
		glTexCoord2f(1, 0);glVertex3f(2000.0f, -500.0f, -1000.0f);
		glTexCoord2f(1, 1);glVertex3f(2000.0f, -500.0f, 2000.0f);
		glTexCoord2f(0, 1);glVertex3f(-1000.0f, -500.0f, 2000.0f);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

void setCursorToCenter()
{
	screen_pos.x = glutGet(GLUT_INIT_WINDOW_X);
	screen_pos.y = glutGet(GLUT_INIT_WINDOW_Y);
	sf::Vector2i ms = sf::Mouse::getPosition();
	if(ms.x >= screen_pos.x && ms.x <= screen_pos.x + screen_size.x)
	{
		if(ms.y >= screen_pos.y && ms.y <= screen_pos.y + screen_size.y)
			sf::Mouse::setPosition(sf::Vector2i(screen_pos.x + screen_size.x / 2, screen_pos.y + screen_size.y / 2));
}

void createWorld()
{
	sf::Image heightmap;
	if(!heightmap.loadFromFile("res/heightmap.png"))
	{
		MessageBox(NULL, "Bir Dosya Açýlamadý", "Hata", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	for(int x = 0; x < 1000; x++)
	{
		for(int z = 0; z < 1000; z++)
		{
			for(int y = 0; y < 30; y++)
			{
				if(x >= 250 && x < 750)
				{
					if(z >= 250 && z < 750)
					{
						if(y == 0) world[x][0][z] = true;
						else world[x][y][z] = false;
					}
				}
			}
		}
	}
}

void drawString(float x, float y, float z, string str)
{
	glRasterPos3f(x, y, z);
	
	for (char c : str)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);  // Updates the position
	}	
}

void drawString2D(float x, float y, string str)
{
	glRasterPos2f(x, y);
	for(char c : str) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
}

sf::Image openFile(string path, bool flip = false)
{
	sf::Image x;
	if(!x.loadFromFile(path))
	{
		MessageBox(NULL, "Bir Dosya Açýlamadý", "Hata", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	if(flip) x.flipVertically();
	return x;
}

void toggleGlutWindowMaximizeBox(char *szWindowTitle)
{
    long dwStyle;
    HWND hwndGlut;
    hwndGlut = FindWindow(NULL, szWindowTitle);
    dwStyle = GetWindowLong(hwndGlut, GWL_STYLE);
    dwStyle ^=WS_MAXIMIZEBOX;
    SetWindowLong(hwndGlut, GWL_STYLE, dwStyle);
}

sf::Vector2f getRotation()
{
	screen_pos.x = glutGet(GLUT_INIT_WINDOW_X);
	screen_pos.y = glutGet(GLUT_INIT_WINDOW_Y);
	sf::Vector2i ms = sf::Mouse::getPosition();
	ms.x -= screen_pos.x;
	ms.y -= screen_pos.y;
	if(ms.x < 0 || ms.y < 0 || ms.x > screen_size.x || ms.y > screen_size.y) return sf::Vector2f(0, 0);
	sf::Vector2f deltaRotation(ms.x - screen_size.x / 2, ms.y - screen_size.y / 2);
	deltaRotation.x /= 1000;
	deltaRotation.y /= 1000;
	return deltaRotation;
}

void HUD_CURSOR()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex2f(screen_size.x / 2 - 8, screen_size.y / 2 - 1);
		glVertex2f(screen_size.x / 2 - 8, screen_size.y / 2 + 1);
		glVertex2f(screen_size.x / 2 + 8, screen_size.y / 2 + 1);
		glVertex2f(screen_size.x / 2 + 8, screen_size.y / 2 - 1);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex2f(screen_size.x / 2 - 1, screen_size.y / 2 - 8);
		glVertex2f(screen_size.x / 2 + 1, screen_size.y / 2 - 8);
		glVertex2f(screen_size.x / 2 + 1, screen_size.y / 2 + 8);
		glVertex2f(screen_size.x / 2 - 1, screen_size.y / 2 + 8);
	glEnd();
}

void drawHudElements()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0f, screen_size.x, 0.0f, screen_size.y);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	//DRAW HUD ELEMENTS HERE
	HUD_CURSOR();
	drawString2D(0, screen_size.y - 50, "X: " + toString(p.getPosition().x));
	drawString2D(0, screen_size.y - 65, "Y: " + toString(p.getPosition().y));
	drawString2D(0, screen_size.y - 80, "Z: " + toString(p.getPosition().z));
	
	drawString2D(0, screen_size.y - 110, "Rotation X: " + toString(Rad2Deg(p.getAngles().x)));
	drawString2D(0, screen_size.y - 125, "Rotation Y: " + toString(Rad2Deg(p.getAngles().y)));
	
	drawString2D(0, screen_size.y - 140, "OnGround: " + toString(p.getOnGround()));
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

int main(int argc, char **argv)
{
	hwnd = GetConsoleWindow();
	SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_size.x, screen_size.y);
	glutInitWindowPosition(screen_pos.x, screen_pos.y);
	glViewport(0, 0, screen_size.x, screen_size.y);
	glutCreateWindow("Minecraft With OpenGL");
	toggleGlutWindowMaximizeBox("Minecraft With OpenGL");
	glutSetCursor(GLUT_CURSOR_NONE);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1000.0f / FPS, timerFunc, 0);
	glutReshapeFunc(resize);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0f, 0.1f, 2500.0f);
	glEnable( GL_POLYGON_SMOOTH );
	
	grass_top = openFile("res/grass/top.jpg");
	grass_side = openFile("res/grass/side.jpg");
	grass_bottom = openFile("res/grass/bottom.jpg");
	
	skybox_top = openFile("res/skybox/skybox_top.bmp", true); // 0 top 
	skybox_side[0] = openFile("res/skybox/skybox_right.bmp"); // 1 right 
	skybox_side[1] = openFile("res/skybox/skybox_back.bmp", true); // 2 back 
	skybox_side[2] = openFile("res/skybox/skybox_left.bmp",true); // 3 left
	skybox_side[3] = openFile("res/skybox/skybox_front.bmp"); // 4 front
	skybox_bottom = openFile("res/skybox/skybox_bottom.bmp"); // bottom
	
	chan_i = openFile("res/jackie.jpg");
	generateTex(chan, chan_i);
	
	createWorld();
	generateTextures();
	
	p.setPosition(500.0f, 15.84f, 500.0f);
    p.setSpeed(5.0f);
    p.setLRSpeed(3.5f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 1.0, 0.0,  // position
			  0.0, 1.0, 5.0,  // looking at
			  0.0, 1.0, 0.0); 
	  
	glutMainLoop();
	return 0;
}
bool once = true;
void display()
{
	sf::Time frame = c.restart();
	p.setFrametime(frame);
	fps ++;
	
	sf::Vector2f deltaRotation = getRotation();
	setCursorToCenter();
	
	p.calculateAngles(deltaRotation.x, -deltaRotation.y);
	if(p.getPosition().y < -15.0f) p.setPosition(p.getPosition().x, 15.84f, p.getPosition().z);
	p.look();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) p.goForward(world);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) p.goBackward();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) p.goLeft();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) p.goRight();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) p.setPosition(p.getPosition().x, p.getPosition().y - 0.1f, p.getPosition().z);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) p.jump();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) p.setSpeed(7.5f);
	else p.setSpeed(5.0f);
	
	fps_t = fps_c.getElapsedTime();
	if(fps_t.asSeconds() >= 1.0f)
	{
		fps_c.restart();
		last_fps = fps;
		fps = 0;
	}
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) lPress = true;
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Right)) rPress = true;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for(int x = p.getPosition().x - dist; x < p.getPosition().x + dist; x++)
	{
		for(int y = 0; y < 30; y++)
		{
			for(int z = p.getPosition().z - dist; z < p.getPosition().z + dist; z++)
			{
				if(x < 0 || x >= 1000) continue;
				if(z < 0 || z >= 1000) continue;
				if(!world[x][y][z]) continue;
				drawCube(x, y, z, 1.0f);
			}
		}
	}
	
	p.checkOnGround(world);
	p.updateY();
	
	float crpn = 4;
	
	drawSkybox();
	
	float dis = 0.0f;
	sf::Vector3i lookingAt = sf::Vector3i(-2, -2, -2);
	sf::Vector3i place = sf::Vector3i(-2, -2, -2);
	bool smt = true;
	while(dis < crpn)
	{
		if(smt)	dis += 0.005;
		else dis -= 0.005f;
		int x = p.getPosition().x + p.getL().x * dis, y = (p.getPosition().y + 1.85 / 2.0f) + p.getL().y * dis, z = p.getPosition().z + p.getL().z * dis;
		
		if(checkW(x, y, z, world) && smt)
		{
			lookingAt = sf::Vector3i(x, y, z);
			smt = false;
		}
		if(y >= 0)
		{
			if(!smt && !checkW(x, y, z, world))
			{
				place = sf::Vector3i(x, y, z);
				break;
			}
		}
		else break;
	}
	
	if(lookingAt != sf::Vector3i(-2, -2, -2))
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && lPress)
		{
			if(world[lookingAt.x][lookingAt.y][lookingAt.z])
			{
				lPress = false;
				world[lookingAt.x][lookingAt.y][lookingAt.z] = false;
			}
		}
	}
	if(place != sf::Vector3i(-2, -2, -2))
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && rPress)
		{
			if(!world[place.x][place.y][place.z] && (place.x != p.getPosition().x && place.y != p.getPosition().y && place.z != p.getPosition().z))
			{
				rPress = false;
				world[place.x][place.y][place.z] = true;
			}
		}
	}
	
	float rot[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	jackie(499, 2, 490, 2, rot);
	glColor3f(1.0, 1.0f, 0.0f);
	glTranslatef(501, 1, 491); 
	glutSolidSphere(.5f, 360, 360);
	glTranslatef(-501, -1, -491);
	
	drawHudElements();
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) glutDestroyWindow(glutGetWindow());
}
void timerFunc(int val)
{
	glutPostRedisplay();
    glutTimerFunc(1000.0f / FPS, timerFunc, 0);
}
void idle()
{
	//p.debug();
}
void resize(int w, int h)
{
	glutReshapeWindow(screen_size.x, screen_size.y);
}
