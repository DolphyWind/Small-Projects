#include "HtmlParser.hpp"

std::vector<std::string> HtmlParser::voidElements = {
    "area",
    "base",
    "basefont",
    "bgsound",
    "br",
    "col",
    "command",
    "embed",
    "frame",
    "hr",
    "image",
    "img",
    "input",
    "isindex",
    "keygen",
    "link",
    "menuitem",
    "meta",
    "nextid",
    "param",
    "source",
    "track",
    "wbr"
};

HtmlTag HtmlParser::parse(std::string input) 
{
    bool inTag = false;
    bool tagNameRead = false;
    std::string data;
    std::string tagData;
    std::string tag;
    bool wasScript = false;
    auto iequals = [](const std::string& a, const std::string& b){
        return std::equal(
            a.begin(), a.end(),
            b.begin(), b.end(),
            [](char a, char b){return std::tolower(a) == std::tolower(b);
            }
        );
    };

    for(std::size_t i = 0; i < input.length(); i++)
    {
        char c = input[i];
        // handle in tag
        if(inTag)
        {
            if(tagNameRead)
            {
                if(c == '>')
                {
                    inTag = false;
                    tagNameRead = false;
                    if(!wasScript || tag == "/script")
                    {
                        wasScript = false;
                        if(tag[0] == '/')
                            handleEndDag(tag.substr(1, tag.length() - 1));
                        else
                        {
                            if(tag.find('!') != std::string::npos) continue;
                            handleStartTag(tag, parseAttributes(tagData.substr(0, tagData.length())));
                            if(tagData[tagData.length() - 1] == '/') handleEndDag(tag);
                            else
                            {
                                for(auto v : voidElements)
                                {
                                    if(v == tag)
                                    {
                                        handleEndDag(tag);
                                        break;
                                    }
                                }
                            }
                        }
                        wasScript = (iequals(tag, "script"));
                    }
                    else if(wasScript)
                    {
                        data += "<" + tag;
                        if(!tagData.empty())
                            data += " " + tagData;
                        data += ">";
                    }
                    
                    tag.clear();
                    tagData.clear();
                }
                else
                    tagData += c;
            }
            else
            {
                if(c == ' ')
                {
                    bool isEmp = true;
                    for(std::size_t i = 0; i < tag.length(); i++)
                    {
                        if(tag[i] != ' ' && tag[i] != '\n' && tag[i] != '\t' && tag[i] != '\r') isEmp = false;
                    }
                    tagNameRead = !isEmp;
                }
                else if(c == '>')
                {
                    inTag = false;
                    tagNameRead = false;
                    if(!wasScript || iequals(tag, "/script"))
                    {
                        if(tag[0] == '/')
                            handleEndDag(tag.substr(1, tag.length() - 1));
                        else
                        {
                            if(tag.find('!') != std::string::npos) continue;
                            handleStartTag(tag, parseAttributes(tagData.substr(0, tagData.length())));
                            if(tagData[tagData.length() - 1] == '/') handleEndDag(tag);
                            else
                            {
                                for(auto v : voidElements)
                                {
                                    if(v == tag)
                                    {
                                        handleEndDag(tag);
                                        break;
                                    }
                                }
                            }
                        }
                        wasScript = iequals(tag, "script");
                    }
                }
                else tag += c;
            }
        }
        else
        {
            if(c != '<')
                data += c;
        }
        if(c == '<')
        {
            tag.clear();
            tagData.clear();
            inTag = true;
            if(!data.empty())
            {
                bool isEmptyData = true;
                for(std::size_t i = 0; i < data.length(); i++)
                {
                    if(data[i] != ' ' && data[i] != '\t' && data[i] != '\n' && data[i] != '\r')
                    {
                        isEmptyData = false;
                        break;
                    }
                }
                if(!isEmptyData)
                {
                    for(std::size_t i = data.length() - 1; i > 0; i--)
                    {
                        if(data[i] == '\t' || data[i] == '\n' || data[i] == ' ' || data[i] == '\r') data.pop_back();
                        else break;
                    }
                    
                    if(!wasScript) handleData(data);
                }
            }
            
            data.clear();
        }
    }

    return mainTag;
}

std::unordered_map<std::string, std::string> HtmlParser::parseAttributes(std::string tagString) 
{   
    if(tagString[tagString.length() - 1] == '/') tagString.pop_back();
    std::unordered_map<std::string, std::string> attrs;
    // Regularize tagString
    for(std::size_t i = 0; i < tagString.length(); i++)
    {
        if(tagString[i] == '=')
        {
            std::size_t deletedLength = 0;
            for(std::size_t j = 1; ; j++)
            {
                if(tagString[i - j] == ' ')
                    tagString.erase(tagString.begin() + i - j);
                else
                {
                    deletedLength = j - 1;
                    break;
                };
            }
            for(std::size_t j = 1; ; j++)
            {
                if(tagString[i + j - deletedLength] == ' ')
                    tagString.erase(tagString.begin() + i + j - deletedLength);
                else break;
            }
        }
    }

    // Parse tags
    std::vector<std::string> attrList;
    std::string attr;
    char quoteType;
    bool inString = false;
    // if in string ignore whitespaces else parse arguments by whitespaces
    for(auto &c : tagString)
    {
        if((c == '\'' || c == '\"') && !inString)
        {
            quoteType = c;
            inString = true;
        }
        else if(inString && c == quoteType)
        {
            inString = false;
        }
        if(c == ' ' && !inString)
        {
            if(!attr.empty())
                attrList.push_back(attr);
            attr = "";
        }
        else
            attr += c;
    }
    attrList.push_back(attr);

    // parse arguments by equals sign
    for(auto &v : attrList)
    {
        std::string first, second;
        bool isEqualsFound = false;
        for(auto &c : v)
        {
            isEqualsFound = (isEqualsFound ? isEqualsFound : c == '=');
            if(!isEqualsFound) first += c;
            else second += c;
        }
        if(second[0] == '=')
            second.erase(second.begin());
        for(std::size_t i = 0; i < second.length(); i++)
        {
            if(second[i] == '\"')
            {
                second.erase(second.begin() + i);
                break;
            }
        }
        for(int i = second.length() - 1; i >= 0; i--)
        {
            if(second[i] == '\"')
            {
                second.erase(second.begin() + i);
                break;
            }
        }
        
        if(!first.empty())
            attrs.insert(std::pair<std::string, std::string>(first, second));
    }
    return attrs;
}

void HtmlParser::handleStartTag(std::string tag, std::unordered_map<std::string, std::string> attrs) 
{
    std::string doctypeStr = "!doctype";
    if(
        std::equal(
            doctypeStr.begin(), doctypeStr.end(),
            tag.begin(), tag.end(),
            [](char a, char b){return std::tolower(a) == std::tolower(b);}
        )
    )
    return;
    std::cout << "Start Tag: " << tag << std::endl;
    if(!attrs.empty()) std::cout << "Attrs:" << std::endl;
    for(auto atr : attrs) std::cout << atr.first << "=" << atr.second << std::endl;
    std::cout << "---------------------------" << std::endl;
    return;
    
    HtmlTag htmlTag;
    htmlTag.setTagname(tag);
    for(auto &atr : attrs)
    {
        htmlTag.addAttribute(atr.first, atr.second);
    }
    auto ptr = htmlTag.clone();
    currentPtr->addChild(ptr);
    currentPtr = ptr.get();
}

void HtmlParser::handleEndDag(std::string tag) 
{
    std::cout << "End Tag: " << tag << std::endl;
    std::cout << "---------------------------" << std::endl;
    return;
    currentPtr = currentPtr->getParent();
}

void HtmlParser::handleData(std::string data) 
{
    bool isEmpty = true;
    for(char &c : data)
        if(c != '\n' && c != ' ' && c != '\t' && c != '\r')
            isEmpty = false;
    if(isEmpty) return;
    std::cout << "Data: " << data << std::endl;
    std::cout << "---------------------------" << std::endl;
    return;
    currentPtr->setText(currentPtr->getText() + data);
}
