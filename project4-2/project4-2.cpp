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

void get_input(char* fName, std::vector< std::vector<int> >& matrix);
std::vector< std::vector<int> > randDistMatrix(int numCities);
std::vector<int> nnTour(std::vector< std::vector<int> > distanceMatrix, int numCities);
int getCost(std::vector< std::vector<int> > distanceMatrix, std::vector<int> tour);

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "Usage: tsp2 filename" << std::endl;
		return 0;
	}

  	clock_t startTime, endTime;
  	startTime = clock();
	
	//Get input
	std::vector< std::vector<int> > distanceMatrix;
  	get_input(argv[1], distanceMatrix);
  	int numCities = distanceMatrix.size();

	//Get nearest neighbor tour
	std::vector<int> tour = nnTour(distanceMatrix, numCities);

	//Open output file
	std::ofstream file;
	std::string fName = argv[1];
	fName += ".tour";
	file.open(fName);
	
	int cost = getCost(distanceMatrix, tour);
	file << cost << '\n';
	
	//Output tour
	std::cout << "Traveling Salesman Tour" << std::endl;
	for(int i = 0; i <= numCities-1; i++)
	{
		std::cout << tour[i] << '\t';
		file << tour[i] << '\n';
	}
	file.close();
	std::cout << std::endl;
	
	std::cout << "Total cost: " << cost << std::endl;
	
	
	endTime = clock();
	endTime -= startTime;
	
	return 0;
}

class point{
  public:
    int number;
    int x;
    int y;
};
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


std::vector< std::vector<int> > randDistMatrix(int numCities)
{
	std::vector< std::vector<int> > distanceMatrix;
	distanceMatrix.resize(numCities, std::vector<int>(numCities, 0));
	int randNum;

	for(int i = 0; i < numCities; i++)
	{
		for(int j = 0; j <= i; j++)
		{
			randNum = std::rand() % 200 + 1;
			if(i == j)
				randNum = -1;
			distanceMatrix[i][j] = randNum;
			distanceMatrix[j][i] = randNum;
		}
	}

	return distanceMatrix;
}


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


int getCost(std::vector< std::vector<int> > distanceMatrix, std::vector<int> tour)
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