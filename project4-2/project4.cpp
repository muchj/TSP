#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <limits>
#include <algorithm>

/*****************************************************************/
/*****************************************************************/
//FUNCTION DECLARATION
/*****************************************************************/
/*****************************************************************/
void get_input(char* fName, std::vector< std::vector<int> >& matrix);
std::vector<int> nnTour(std::vector< std::vector<int> > distanceMatrix, int numCities);
int calculateCost(std::vector< std::vector<int> > distanceMatrix, std::vector<int> tour);
std::vector<int> two_opt(std::vector<int> nnTour, std::vector< std::vector<int> > distanceMatrix, int numCities, clock_t timer);
std::vector<int> two_optSwap(std::vector<int> nnTour, int swapFront, int swapEnd, int numCities);

/*****************************************************************/
/*****************************************************************/
//CLASS DECLARATION
/*****************************************************************/
/*****************************************************************/
class point{
  public:
    int number;
    int x;
    int y;
};

/*****************************************************************/
/*****************************************************************/
//MAIN FUNCTION
/*****************************************************************/
/*****************************************************************/
int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "Usage: tsp2 filename" << std::endl;
		return 0;
	}

	//for testing
  	clock_t startTime, endTime;
  	startTime = clock();
	
	//Get input
	std::vector< std::vector<int> > distanceMatrix;
  	get_input(argv[1], distanceMatrix);
  	int numCities = distanceMatrix.size();

	//Get nearest neighbor tour
	std::vector<int> tour = nnTour(distanceMatrix, numCities);

	//Output tour
	std::cout << "Nearest Neighbor Tour" << std::endl;
	for(int i = 0; i <= numCities-1; i++)
	{
		std::cout << tour[i] << '\t';
	}
	std::cout << std::endl;

	//Get and output cost
	int cost = calculateCost(distanceMatrix, tour);
	std::cout << "Total cost: " << cost << std::endl;

	//Get 2opt tour
	std::vector<int> two_optTour = two_opt(tour, distanceMatrix, numCities, startTime);

	//Open output file
	std::ofstream file;
	std::string fName = argv[1];
	fName += ".tour";
	file.open(fName);

	//Get and output cost
	cost = calculateCost(distanceMatrix, two_optTour);
	file << cost << '\n';

	//Output tour
	std::cout << "After 2-opt and 2-opt Swap" << std::endl;
	for(int i = 0; i <= numCities-1; i++)
	{
		std::cout << two_optTour[i] << '\t';
		file << two_optTour[i] << '\n';
	}
	std::cout << std::endl;

  	file.close();
	
	//for testing
	endTime = clock();
	endTime -= startTime;

	//Output cost
	cost = calculateCost(distanceMatrix, two_optTour);
	std::cout << "Total cost: " << cost << std::endl;

	return 0;
}
/*****************************************************************/
/*****************************************************************/
//ALGORITHMS AND HELPER FUNCTIONS
/*****************************************************************/
/*****************************************************************/

//Putting file data into a point matrix
void get_input(char* fName, std::vector< std::vector<int> >& matrix){
	std::vector<point> pointList;
	std::ifstream file;
	std::string line;
	file.open(fName);
	
	while (std::getline(file, line)){
    std::istringstream iss(line);
    point a;
    iss >> a.number;
    iss >> a.x;
    iss >> a.y;
    pointList.push_back(a);
  }
  file.close();

  // Turns list of points into relative distance matrix
  int numPoints = pointList.size();
  matrix.resize(numPoints, std::vector<int>(numPoints, -1));
  
  for (int a = 0; a < numPoints; a++){
    for (int b = 0; b < numPoints; b++){
      matrix[a][b] = round(sqrt((pointList[a].x - pointList[b].x)*(pointList[a].x - pointList[b].x) + (pointList[a].y - pointList[b].y)*(pointList[a].y - pointList[b].y)));
    }
  }
}

//Salesman tour solver using neighest neighbor method. 
//Source: https://en.wikipedia.org/wiki/Nearest_neighbour_algorithm
std::vector<int> nnTour(std::vector< std::vector<int> > distanceMatrix, int numCities)
{
	int currentCity, nextCity;
	int minDistance;
	std::vector<int> tour;
	tour.resize(numCities + 1, -1);	
	currentCity = 0;

	for(int j = 0; j < numCities; j++)
	{
		minDistance = std::numeric_limits<int>::max();
		tour[j] = currentCity;
		for(int i = 0; i < numCities; i++)
		{
			if(i != currentCity && distanceMatrix[currentCity][i] < minDistance && !(std::find(tour.begin(), tour.end(), i) != tour.end()))
			{
				minDistance = distanceMatrix[currentCity][i];
				nextCity = i;
			}
		}
		currentCity = nextCity;
	}
	tour[numCities] = 0;	
	return tour;
}

//We pass the nearest neighbor solved matrix into the 2-opt and 2-optSwap functions to find a more precise function.
//Source: https://en.wikipedia.org/wiki/2-opt
//Source: http://www.technical-recipes.com/2012/applying-c-implementations-of-2-opt-to-travelling-salesman-problems/
std::vector<int> two_opt(std::vector<int> nnTour, std::vector< std::vector<int> > distanceMatrix, int numCities, clock_t timer)
{
	
	int size = nnTour.size();
	bool improve = false;
	std::vector<int> finalTour = nnTour;
  	clock_t max_time = CLOCKS_PER_SEC * 30;
  	clock_t loop_time;
  	clock_t current_time;
	
	while (!improve && timer < max_time)
	{
		int best_dist = calculateCost(distanceMatrix, finalTour);
		for (int i = 1; i < size - 2; i++)
		{
			for (int k = i+1; k < size - 1; k++)
			{
				std::vector<int> newTour = two_optSwap(finalTour, i, k, numCities);
				int new_dist = calculateCost(distanceMatrix, newTour);

				if(new_dist < best_dist)
				{

					improve = false;
					finalTour = newTour;
					best_dist = new_dist;
				}

				current_time = clock();
				loop_time = current_time - timer;
				if(loop_time > max_time)
				{
					std::cout << "\nTime to complete Neighest Neighbor and 2-Opt(sec): " << (loop_time / CLOCKS_PER_SEC) << std::endl;
					return finalTour;
				}
			}
		}
		improve = true;
	}
  	std::cout << "\nTime to complete Neighest Neighbor and 2-Opt (sec): " << (loop_time / CLOCKS_PER_SEC) << std::endl;
	return finalTour;
}

//Second part of 2-opt solution. This will swap tour stops if one provides a lower cost.
//Same sources as above.
std::vector<int> two_optSwap(std::vector<int> nnTour, int swapFront, int swapEnd, int numCities)
{
	int size = nnTour.size();
	std::vector<int> newTour = nnTour;

	for (int i = 0; i < swapFront - 1; i++)
	{
		newTour[i] = nnTour[i];
	}

	int decVar = 0;
	for (int i = swapFront; i <= swapEnd; i++)
	{
		newTour[i] = nnTour[swapEnd - decVar];
		decVar++;
	}

	for (int i = swapEnd + 1; i < size; i++)
	{
		newTour[i] = nnTour[i];
	}
	return newTour;
}

//Function to calculate total cost
int calculateCost(std::vector< std::vector<int> > distanceMatrix, std::vector<int> tour)
{
	int cost, totalCost;
	int tourLength = tour.size();
	int currentCity, nextCity;
	totalCost = 0;

	for(int i = 0; i < tourLength - 1; i++)
	{
		currentCity = tour[i];
		nextCity = tour[i+1];
		cost = distanceMatrix[currentCity][nextCity];
		totalCost += cost;
	}
	return totalCost;
}