#include <iostream>

// https://codeforces.com/problemset/problem/110/A

int main(int argc, char *argv[])
{
	std::string number;
	std::cin >> number;
	int luckyCount = 0;

	for(auto &c : number)
	{
		if(c == '4' || c == '7') luckyCount ++;
	}
	if(luckyCount == 4 || luckyCount == 7) std::cout << "YES";
	else std::cout << "NO";
	return 0;
}