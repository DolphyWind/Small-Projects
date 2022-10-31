#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

enum class Answer
{
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    COUNT
};

class Question
{
    sf::String question;
    sf::String answers[(std::size_t)Answer::COUNT];
    Answer rightAnswer;
public:
    Question() {};
    Question(sf::String question, sf::String ans0, sf::String ans1, sf::String ans2, sf::String ans3, Answer rightAnswer);
    void setQuestion(sf::String q);
    sf::String getQuestion();
    void setAnswer(Answer answer, sf::String text);
    sf::String getAnswer(Answer answer);
    void setRightAnswer(Answer answer);
    Answer getRightAnswer();
    void shuffleAnswers(int n);

    ~Question() {};
};