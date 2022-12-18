#include "HtmlTag.hpp"

HtmlTag::HtmlTag(std::string tagname) 
{
    this->tagname = tagname;
}

void HtmlTag::setText(const std::string newText) 
{
    text = newText;
}

std::string HtmlTag::getText() 
{
    std::string result = text;
    if(!children.empty()) result += '\n';
    for(auto& c : children)
        result += c->toString(1);
    return result;
}

std::string HtmlTag::getPureText() 
{
    std::string result = text;
    for(auto& c : children)
        result += c->getPureText();
    return result;
}

std::string HtmlTag::getTextWithoutChildren() 
{
    return text;
}

void HtmlTag::setTagname(std::string tname) 
{
    this->tagname = tname;
}

std::string HtmlTag::getTagname() 
{
    return tagname;
}

std::shared_ptr<HtmlTag> HtmlTag::clone() 
{
    //return new HtmlTag(*this);
    return std::shared_ptr<HtmlTag>(new HtmlTag(*this));
}

void HtmlTag::addChild(std::shared_ptr<HtmlTag> htmlTag) 
{
    children.push_back(htmlTag);
    htmlTag->parent = this;;
}

std::vector<std::shared_ptr<HtmlTag>> *HtmlTag::getChildren() 
{
    return &children;
}

std::shared_ptr<HtmlTag> HtmlTag::getChild(std::size_t index) 
{
    return children.at(index);
}

HtmlTag* HtmlTag::getParent() 
{
    return this->parent;
}

void HtmlTag::addAttribute(std::string name, std::string value) 
{
    attributes.insert(std::pair<std::string, std::string>(name, value));
}

void HtmlTag::addAttribute(std::string name) 
{
    attributes.insert(std::pair<std::string, std::string>(name, ""));
}

const std::string &HtmlTag::getAttribute(std::string name) 
{
    return attributes[name];
}

bool HtmlTag::hasAttribute(std::string name) 
{
    try
    {
        attributes.at(name);
        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

const std::unordered_map<std::string, std::string>& HtmlTag::getAllAttributes() 
{
    return attributes;
}

std::string HtmlTag::toString(std::size_t tabbing) 
{
    std::string result;
    result += makeTab(tabbing) + "<" + tagname;
    for(auto& itr : attributes)
    {
        result += " " + itr.first;
        if(!itr.second.empty())
            result += "=\"" + itr.second + "\"";
    }
    result += ">";
    if(text.find('\n') != std::string::npos) result += "\n";
    result += makeTab(text, tabbing + 1);
    if(text.find('\n') != std::string::npos) result += "\n";
    if(!children.empty()) result += "\n";
    for(auto& itr : children)
    {
        result += itr->toString(tabbing + 1);
    }
    if(text.find('\n') != std::string::npos || !children.empty()) result += makeTab(tabbing);
    result += "</" + tagname + ">\n";
    return result;
}

std::vector<std::shared_ptr<HtmlTag>> HtmlTag::findAll(std::string tag) 
{
    std::vector<std::shared_ptr<HtmlTag>> result;
    for(auto& c : children)
    {
        for(auto t : c->findAll(tag))
            result.push_back(t);
        if(c->getTagname() == tag) result.push_back(c);
    }
    return result;
}

std::vector<std::shared_ptr<HtmlTag>> HtmlTag::findAll(std::string tag, std::unordered_map<std::string, std::string> attrs)
{
    std::vector<std::shared_ptr<HtmlTag>> result;
    for(auto& c : children)
    {
        for(auto t : c->findAll(tag, attrs))
            result.push_back(t);
        if(c->getTagname() == tag)
        {
            bool temp = true;
            for(auto &attr : attrs)
            {
                if(c->getAttribute(attr.first) != attr.second)
                {
                    temp = false;
                    break;
                }
            }
            if(temp) result.push_back(c);
        }
    }
    return result;
}

const std::string& HtmlTag::operator[](const std::string name) 
{
    return getAttribute(name);
}

std::string HtmlTag::makeTab(std::size_t count) 
{
    std::string result;
    for(std::size_t i = 0; i < count; i++)
        result.push_back('\t');
    return result;
}

std::string HtmlTag::makeTab(std::string str, std::size_t count) 
{
    std::string result;
    for(std::size_t i = 0; i < str.length(); i++)
    {
        result.push_back(str[i]);
        if(str[i] == '\n') result += makeTab(count);
    }
    if(str.find("\n") == std::string::npos)
        return result;
    else
        return makeTab(count) + result;
}
