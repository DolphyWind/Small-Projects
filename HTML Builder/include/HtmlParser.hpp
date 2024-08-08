#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "HtmlTag.hpp"

class HtmlParser
{
private:
    static std::vector<std::string> voidElements;
    std::unordered_map<std::string, std::string> parseAttributes(std::string tagString);
    HtmlTag mainTag;
    HtmlTag* currentPtr = &mainTag;
public:
    HtmlParser() {};
    ~HtmlParser() {};
    HtmlTag parse(std::string input);

    virtual void handleStartTag(std::string tag, std::unordered_map<std::string, std::string> attrs);
    virtual void handleEndDag(std::string tag);
    virtual void handleData(std::string data);
};