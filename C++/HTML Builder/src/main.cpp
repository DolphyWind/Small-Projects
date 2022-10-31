#include <iostream>
#include <fstream>
#include <sstream>
#include "HtmlTag.hpp"
#include "HtmlParser.hpp"
#include "Request.hpp"

int main()
{
	std::string sourceCode;
	std::ifstream file("src.html");
	std::stringstream strStream;
	strStream << file.rdbuf();
	sourceCode = strStream.str();
	file.close();
	/*std::string sourceCode;
	Request req;
	sourceCode = req.get("https://bigpara.hurriyet.com.tr/doviz/dolar/");
	std::cout << sourceCode << std::endl;*/
	HtmlParser parser;
	HtmlTag code = parser.parse(sourceCode);
	//std::cout << code.toString() << std::endl;

	for(auto &e : code.findAll("p"))
	{
		for(auto &l : e->findAll("a"))
		{
			std::cout << l->getAttribute("href") << std::endl;
			std::cout << "--------------------------" << std::endl;
		}
		
	}

	return 0;
}
