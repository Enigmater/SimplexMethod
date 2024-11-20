#pragma once
#include <iostream> 
#include <vector>
#include <limits>
#include <iomanip>

using namespace std;

class SimplexMethod
{
private:
	std::vector<std::vector<double>> table;
	int numVariables; 

	vector<int> basicVariables, nonBasicVariables;

	int GetPivotColumn();
	int GetPivotRow(int pivotCol);
	void Pivot(int pivotRow, int pivotCol);

public:
	SimplexMethod(std::vector<std::vector<double>> table, int numVariables);

	void Solve();
	void GetResult();
	void PrintTable();
};

