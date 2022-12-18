#include <iostream>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

vector<string> getAllFiles()
{
	vector<string> files;
	DIR *dir;
	struct dirent *ent;
	if((dir = opendir ("./population/")) != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
			files.push_back(ent->d_name);
	  	closedir (dir);
	}
	else
	{
		perror ("");
		return files;
	}
	vector<string> a;
	for(int i = 0; i < files.size(); i++) 
	{
		if(files[i] != "." && files[i] != "..") a.push_back(files[i]);
	}
	return a;
}

template<typename T>
void deleteElementByIndex(vector<T> &v, int index)
{
	v.erase(v.begin() + index);
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
	string default_dir = getDefaultDir(argv[0]);
	vector<string> files = getAllFiles();

	int destroy = 0;
	destroy = files.size() / 2;

	for(int i = 0; i < destroy;)
	{
		unsigned int r = rand()%files.size();
		switch(remove((default_dir + "/population/" + files[r]).c_str()))
		{
			case 0:
			{
				i++;
				deleteElementByIndex(files, r);
			}
				break;
			default:
				break;
		}
	}
}
