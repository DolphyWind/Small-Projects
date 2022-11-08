#include <iostream>

// https://codeforces.com/problemset/problem/59/A

int main(int argc, char *argv[])
{
	std::string word;
	std::cin >> word;
	int lc = 0, uc = 0;
	for(auto &c : word)
	{
		if(std::isupper(c)) uc ++;
		else lc ++;
	}
	if(lc >= uc)
	{
		for(int i = 0; i < word.length(); i++) word[i] = std::tolower(word[i]);
	}
	else
	{
		for(int i = 0; i < word.length(); i++) word[i] = std::toupper(word[i]);
	}
	std::cout << word;
	return 0;
}