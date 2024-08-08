#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Button.hpp"
#include "UIElement.hpp"
#include "font_data.hpp"
#include "button_data.hpp"
#include "question_field_data.hpp"
#include "Question.hpp"
#include <filesystem>
#include <random>
#define TIMER_RADIUS 15
#ifdef _WIN32
#define uint unsigned int
#else
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#endif // _WIN32

enum class Scene
{
    MainMenu = 0,
    Ingame = 1,
    EndScreen = 2
};

class MainWindow
{
private:
    bool lPress = true;
    sf::RenderWindow window;
    sf::String windowTitle;
    sf::Vector2u windowSize;
    sf::Uint32 windowStyle;
    sf::Event e;
    sf::Font font;
    Button questionField;
    Button optionA, optionB, optionC, optionD;
    Button playButton, exitButton;
    Button restartButton, mainMenuButton, exitButtonE;
    sf::Text endScreenText[3];
    sf::Text titleText;
    sf::Text scoreText;
    sf::Texture buttonTexture;
    sf::Texture questionFieldTexture;
    sf::ConvexShape timerShape;
    sf::CircleShape outerTimer;
    sf::Color originalBackgroundColor = sf::Color(38, 16, 89);
    sf::Color backgroundColor = originalBackgroundColor;
    sf::Clock timerClock;
    sf::Text timerText;
    Scene currentScene = Scene::MainMenu;
    bool timerAnim = false;
    int measuredAsIntPrev = 0;
    std::int64_t score = 0;
    std::int64_t highScore = 0;
    float maxTime = 60;
    std::vector<std::unique_ptr<Question>> questions;
    int currentIndex = 0;
    bool btnPress = true;
    bool lrPress = true;

    const char *homedir;
    std::string homedirStr;
    std::string saveFolderPath;
    std::string filePath;

    bool isGrading = false;
    sf::Color targetColor;
    bool isBackwards = false;
    double distanceGraded = 0;
    const double gradingSpeed = 1.f / 15.f;
    sf::Color color_right = sf::Color(34, 158, 3);
    sf::Color color_wrong = sf::Color(191, 2, 2);

    void handleEvents();
    void update();
    void draw();
    void createAnswerButton(Button &btn, sf::String str);
    void createMainMenuButtons(Button &btn, sf::String str);
    void createTimerShape();
    void createTimerText();
    void calcPosTimerText();
    void createUIButtons();
    void loadAssets();
    void updateTimerShape();
    void createIngame();
    void updateIngame();
    void drawIngame();
    void createMainMenu();
    void updateMainMenu();
    void drawMainMenu();
    void createEndScreen();
    void updateEndScreen();
    void drawEndScreen();
    void createEndText();
    void createQuestions();
    void selectQuestion(std::size_t index);
    void pickedRight();
    void pickedWrong();
    void saveHighscore();
    std::size_t generateRandomIndex();
    void createScoreText();
    void gradeFunction();
    void startGrading(bool toGreen);
public:
    MainWindow(sf::Vector2u size, sf::String title, sf::Uint32 style);
    ~MainWindow();
    
    void mainLoop();
};