#ifndef _HTML_TAG_HPP_
#define _HTML_TAG_HPP_
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <algorithm>
#include <regex>
#include <tuple>
#include <memory>

class HtmlTag
{
private:
    std::vector<std::shared_ptr<HtmlTag>> children;
    std::unordered_map<std::string, std::string> attributes;
    std::string text;
    std::string tagname;
    std::string makeTab(std::size_t count = 0);
    std::string makeTab(std::string str, std::size_t count = 0);
    HtmlTag* parent = nullptr;
public:
    HtmlTag(std::string tagname);
    HtmlTag() {};
    ~HtmlTag() {};

    void setText(const std::string newText);
    std::string getText();
    std::string getPureText();
    std::string getTextWithoutChildren();

    void setTagname(std::string tname);
    std::string getTagname();

    std::shared_ptr<HtmlTag> clone();

    void addChild(std::shared_ptr<HtmlTag> htmlTag);
    std::vector<std::shared_ptr<HtmlTag>> *getChildren();
    std::shared_ptr<HtmlTag> getChild(std::size_t index);
    HtmlTag* getParent();

    void addAttribute(std::string name, std::string value);
    void addAttribute(std::string name);
    const std::string &getAttribute(std::string name);
    bool hasAttribute(std::string name);
    const std::unordered_map<std::string, std::string> &getAllAttributes();

    std::vector<std::shared_ptr<HtmlTag>> findAll(std::string tag);
    std::vector<std::shared_ptr<HtmlTag>> findAll(std::string tag, std::unordered_map<std::string, std::string> attrs);

    std::string toString(std::size_t tabbing = 0);

    // Operators
    const std::string &operator[](const std::string name);
};

#endif // !_HTML_TAG_HPP_