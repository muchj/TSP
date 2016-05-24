/*
CS325 - Project 2 
Contributors - Joseph Cuellar, Adam Much, Ryan Peters
Instructions: 
*/

#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <climits>


#define MAX_GRID_SIZE 40

using namespace std;

struct city {
    int id;
    int x;
	int y;
};

////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////
int inputFileLineCount(string inputFile);
void parseInputFile(string inputFileName, int numCities, city cityData[], int distanceGrid[][MAX_GRID_SIZE]);
string createOutputFile(string inputFile);
void printCityData(int numCities, city cityData[], int distanceGrid[][MAX_GRID_SIZE]);

void testRunTime(); // Not yet implemented.
void output(); // Not yet implemented.

////////////////////////////////////////////
// Algorithm Prototypes
////////////////////////////////////////////


////////////////////////////////////////////
// MAIN
////////////////////////////////////////////
int main(int argc, char *argv[])
{
	string inputFileName;
	string outputFileName;
	int numCities;

	// If a file name is not given, prompt for it.
	if (argc < 2)
	{
		cout << "Which file do you wish to test? ";		
		cin >> inputFileName;		
	}
	
	// Otherwise use the given name at argv[1];
	else	
		inputFileName = string(argv[1]);	
	
	// Create the output file by adding ".tour" onto the end of the input file name. 	
	outputFileName = createOutputFile(inputFileName);

	// The number of cities in the file will correspond to the number of lines.
	numCities = inputFileLineCount(inputFileName);
	cout << "Cities: " << numCities << endl; // used for testing.

	// Array of city structures to hold the triple int id, x, y.
	city cityData[numCities];

	// Array of ints to hold distances between cities.
	int distanceGrid[numCities][MAX_GRID_SIZE];
	
	// Populate the arrays.	
	parseInputFile(inputFileName, numCities, cityData, distanceGrid);	

	printCityData(numCities, cityData, distanceGrid); // Used for testing.

	/***********************************************************
	
				RUN ALGORITHMS HERE
	
	************************************************************/

	return 0;
}

////////////////////////////////////////////
// 	parseInputFile
//	- Accepts the input file name, number of cities, 1-D array for city 
//	- structs, and a 2-D array  for city distances. 
//	- Reads data from the file and fills in the data for the 1-D and 
//	- 2-D arrays.
////////////////////////////////////////////
void parseInputFile(string inputFileName, int numCities, city cityData[], int distanceGrid[][MAX_GRID_SIZE])
{
	ifstream inputFile(inputFileName.c_str());
	int nextInt;
	int currentCity = 0;
	double distance;
	
	if (inputFile)
	{
		// Assign values in order and update currentCity on each run.
		while(!inputFile.eof())
		{			
			inputFile >> nextInt;
				cityData[currentCity].id = nextInt;
			
			inputFile >> nextInt;
				cityData[currentCity].x = nextInt;

			inputFile >> nextInt;
				cityData[currentCity].y = nextInt;

			currentCity++;		
		}
		inputFile.close();		

		// Calculate distances.
		for (int i = 0; i < numCities; i++)
		{
			for (int j = 0; j < numCities; j++)
			{
				if (i == j)
					distanceGrid[i][j] = 0;

				else
				{
					distance = sqrt(pow(cityData[i].x - cityData[j].x, 2.0) +
									pow(cityData[i].y - cityData[j].y, 2.0));

					distanceGrid[i][j] = round(distance);
				}
			}
		}		
	}

	else
		cout << "\nUnable to open " << inputFileName << "." << endl;
	
}

////////////////////////////////////////////
// 	createOutputFile
//	- Accepts a string that corresponds to the input file name.
//  - Returns an appropriately named string to be used as the output file name.
////////////////////////////////////////////
string createOutputFile(string inputFile)
{
	string opFile = inputFile;	
	opFile.insert(opFile.length(), ".tour");	

	return opFile;
}

////////////////////////////////////////////
//	inputFileLineCount
//	- Gets the line count of the input file.
////////////////////////////////////////////
int inputFileLineCount(string inputFileName)
{
	int lineCount = 0;
	string line;
	ifstream inputFile(inputFileName.c_str());
	while (inputFile.good())
	{
		getline(inputFile, line);		
		if(line == "")
		{
			break;
		}
		else
		{
			lineCount++;
		}
	}	
	inputFile.close();

	return lineCount;
}

////////////////////////////////////////////
//	printCityData
//	- Prints the array of city structures and the 2-D array of distances
////////////////////////////////////////////
void printCityData(int numCities, city cityData[], int distanceGrid[][MAX_GRID_SIZE])
{
	// Print coordinates.
	cout << endl;
	for (int i = 0; i < numCities; i++)
	{
		cout << "City id: " << cityData[i].id << " Coordinates: (" << cityData[i].x;
		cout << ", " << cityData[i].y << ")\n";
	}

	// Print distances.
	cout << "\nDistances:\n\t";
	for (int i = 0; i < numCities; i++) // City id's in first row.
	{
		cout << cityData[i].id << "\t";
	}
	cout << endl;

	for (int j = 0; j < numCities; j++)
	{
		cout << cityData[j].id << "\t"; // City id's at beginning each column.
		for (int i = 0; i < numCities; i++)
		{
			cout << distanceGrid[i][j] << "\t";
		}
		cout << endl;
	}
}