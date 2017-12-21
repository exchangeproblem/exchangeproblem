
#include "stdafx.h"
#include <vector>

using namespace std;

class CFloydWarshell
{
public:
	void FloydWarshell(std::vector<std::vector<double>> &adjMatrix, int NumofVertices, 
		std::vector<std::vector<double>> &cost, std::vector<std::vector<double>> &path);

	// Function to print path information between two vertices
	void getConnectedPath(std::vector<std::vector<double>> &path, int startNode, int endNode, vector<int> &pathList);

private:
	// Recurive Function to print path of given vertex u from source vertex v
	void printPath(std::vector<std::vector<double>> &path, int v, int u, vector<int> &pathList);
};