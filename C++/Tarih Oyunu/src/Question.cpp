#include "Question.hpp"

Question::Question(sf::String question, sf::String ans0, sf::String ans1, sf::String ans2, sf::String ans3, Answer rightAnswer) 
{
    this->setQuestion(question);
    this->setAnswer(Answer::A, ans0);
    this->setAnswer(Answer::B, ans1);
    this->setAnswer(Answer::C, ans2);
    this->setAnswer(Answer::D, ans3);
    this->setRightAnswer(rightAnswer);
}

void Question::setQuestion(sf::String q) 
{
    this->question = q;
}

sf::String Question::getQuestion() 
{
    return this->question;
}

void Question::setAnswer(Answer answer, sf::String text) 
{
    this->answers[(std::size_t)answer] = text;
}

sf::String Question::getAnswer(Answer answer) 
{
    return this->answers[(std::size_t)answer];
}

void Question::setRightAnswer(Answer answer) 
{
    this->rightAnswer = answer;
}

Answer Question::getRightAnswer() 
{
    return this->rightAnswer;
}

void Question::shuffleAnswers(int n) 
{
    for(int i = 0; i < n; i++)
    {
        int i1, i2;
        do
        {
            i1 = rand()%(int)Answer::COUNT;
            i2 = rand()%(int)Answer::COUNT;
        } while (i1 == i2);
        sf::String ans1 = answers[i1];
        answers[i1] = answers[i2];
        answers[i2] = ans1;
        if(i1 == (int)rightAnswer)
            rightAnswer = (Answer)i2;
        else if(i2 == (int)rightAnswer)
            rightAnswer = (Answer)i1;
    }
}