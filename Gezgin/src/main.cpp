#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#define POINT_COUNT 7

struct point
{
	int x;
	int y;
};
point points[POINT_COUNT];
float distanceTable[POINT_COUNT][POINT_COUNT];
int shortest[POINT_COUNT];

float distanceBetween(point &p1, point &p2)
{
	return hypot(
		std::fabs( (float) p2.x - (float)p1.x),
		std::fabs( (float) p2.y - (float)p1.y)
	);
}

void getInput()
{
	for(int i = 0; i < POINT_COUNT; i++)
	{
		std::cout << "p[" << i << "].x = ";
		std::cin >> points[i].x;
		std::cout << "p[" << i << "].y = ";
		std::cin >> points[i].y;
	}
}

void createDistanceTable()
{
	for(int i = 0; i < POINT_COUNT; i++)
	{
		for(int j = 0; j < POINT_COUNT; j++)
			distanceTable[i][j] = distanceBetween(points[i], points[j]);
	}
}

void solve()
{
	float shortest_distance = std::numeric_limits<float>::max();
	int arr[] = {0, 1, 2, 3, 4, 5, 6};
	do
	{
		float total_dist = 0;
		for(int i = 0; i < POINT_COUNT - 1; i++)
			total_dist += distanceTable[arr[i]][arr[i + 1]];
		if(total_dist < shortest_distance)
		{
			shortest_distance = total_dist;
			for(int i = 0; i < POINT_COUNT; i++)
				shortest[i] = arr[i];
		}
	} while (std::next_permutation(arr, arr + POINT_COUNT));
	std::cout << "----------------------------" << std::endl << "Path: ";
	for(int i = 0; i < POINT_COUNT; i++) std::cout << shortest[i] << " ";
	std::cout << std::endl << "Total Distance: " << shortest_distance << std::endl;
}

int main()
{
	getInput();
	createDistanceTable();
	solve();
	return 0;
}