#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sstream>
using namespace std;

template<typename T>
string toString(T arg)
{
	stringstream ss;
	ss << arg;
	return ss.str();
}

string getDefaultDir(string argv_0)
{
	bool x = false;
	string rev_dir, dir;
	for(int i = argv_0.length() - 1; i >= 0; i--)
	{
		if(!x)
		{
			if(argv_0[i] == '/' || argv_0[i] == '\\') x = true;
		}
		else
		{
			rev_dir += argv_0[i];
		}
	}
	for(int i = rev_dir.length() - 1; i >= 0; i--)
		dir += rev_dir[i];
	return dir;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int n = 0;
	int max = 10000;
	cout << "Enter the number of files you want to create( 0 for random, max 10000 ): ";
	cin >> n;
	while(n == 0) n = rand()%max;
	if(n > max) n = max;
	string default_dir = getDefaultDir(argv[0]);
	for(int i = 1; i <= n; i++)
	{
		ofstream ost(default_dir + "/population/human_" + toString(i) + ".txt");
		ost << "";
		ost.close();
	}
	cout << endl << "Humans created succesfully!" << endl;
	return 0;
}
