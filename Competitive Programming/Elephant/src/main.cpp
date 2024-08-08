#include <iostream>

// https://codeforces.com/problemset/problem/617/A

int main(int argc, char *argv[])
{
	int x;
	int step = 5;
	std::cin >> x;
	int stepCount = 0;

	while(step > 0)
	{
		if(x >= step)
		{
			x -= step;
			stepCount++;
		}
		else
			step--;
	}
	std::cout << stepCount;
	return 0;
}