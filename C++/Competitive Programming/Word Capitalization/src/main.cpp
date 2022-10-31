#include <iostream>

int main(int argc, char *argv[])
{
	std::string inp;
	std::cin >> inp;
	inp[0] = toupper(inp[0]);
	std::cout << inp;
}
/*
Capitalization is writing a word with its first letter as a capital letter. Your task is to capitalize the given word.

Note, that during capitalization all the letters except the first one remains unchanged.
Input

A single line contains a non-empty word. This word consists of lowercase and uppercase English letters. The length of the word will not exceed 10^3.
Output

Output the given word after capitalization.
*/