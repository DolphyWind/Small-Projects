#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <Electricity.hpp>
#include <sstream>
#include <cmath>

class Electra
{
private:
    std::vector<double> m_vector; // Vector
    std::vector<Electricity> m_electricity;
    double m_currentValue = 0;
    int width, height;
    VVC m_code;

    std::vector<std::string> split(const std::string& str, const std::string& delim);
    void readSourceCode(const std::string &filename);
    bool isHorizontal(Electricity &e);
    bool isVertical(Electricity &e);
    void fourWayMove(Electricity &e, std::vector<Electricity> *v);
    void printVector();
    
public:
    Electra(std::string filename);
    void step();
    void createFirstOnes();
    void move(Electricity &e);
    ~Electra() {};
};
