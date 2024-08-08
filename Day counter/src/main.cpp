#include <iostream>
#include <ctime>
#include <cmath>

int main(int argc, char *argv[])
{
	std::time_t now = time(0);
	int year, month, day;
	std::cout << "Doğum Yılı: ";
	std::cin >> year;
	std::cout << "Doğum Ayı: ";
	std::cin >> month;
	std::cout << "Doğum Günü: ";
	std::cin >> day;
	
	std::tm birthdate = {0};
	birthdate.tm_year = year - 1900;
	birthdate.tm_mon = month - 1;
	birthdate.tm_mday = day;

	std::time_t btime = std::mktime(&birthdate);
	const int seconds_per_day = 60 * 60 * 24;
	double diff = std::difftime(now, btime) / seconds_per_day;
	diff = std::abs(diff);
	std::cout << diff << " gündür hayattasınız." << std::endl;
}