#include <bits/stdc++.h>

int main(int argc, char *argv[])
{
	int n = 0;
	std::cin >> n;
	int ppl = 0, pplmax = 0;
	while(n --)
	{
		int a, b;
		std::cin >> a >> b;
		ppl -= a;
		ppl += b;
		if(ppl > pplmax) pplmax = ppl;
	}
	std::cout << pplmax;
}