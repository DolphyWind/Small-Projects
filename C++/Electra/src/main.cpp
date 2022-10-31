#include <iostream>
#include <Electra.hpp>

int main(int argc, char *argv[])
{
	if(argc == 1)
    {
        std::cout << "ERROR: No source code!" << std::endl;
        return 1;
    }
	Electra electra(argv[1]);
	return 0;
}