
#include "stdafx.h"
#include "FloydWarshell.h"

// Function to run Floyd-Warshell algorithm
void CFloydWarshell::FloydWarshell(std::vector<std::vector<double>> &adjMatrix, int NumofVertices, 
	std::vector<std::vector<double>> &cost, std::vector<std::vector<double>> &path)
{
	// initialize cost[] and parent[]
	for (int v = 0; v < NumofVertices; v++)
	{
		for (int u = 0; u < NumofVertices; u++)
		{
			// initally cost would be same as weight 
			// of the edge
			cost[v][u] = adjMatrix[v][u];

			if (v == u)
				path[v][u] = 0;
			else if (cost[v][u] != DBL_MAX)
				path[v][u] = v;
			else
				path[v][u] = -1;
		}
	}

	// run Floyd-Warshell
	for (int k = 0; k < NumofVertices; k++)
	{
		for (int v = 0; v < NumofVertices; v++)
		{
			for (int u = 0; u < NumofVertices; u++)
			{
				// If vertex k is on the shortest path from v to u,
				// then update the value of cost[v][u], path[v][u]

				if (cost[v][k] != DBL_MAX && cost[k][u] != DBL_MAX
					&& cost[v][k] + cost[k][u] < cost[v][u])
				{
					cost[v][u] = cost[v][k] + cost[k][u];
					path[v][u] = path[k][u]; //slight modifiction to FloydWarshell to store path information
				}
			}

			// if diagonal elements become negative, the
			// graph contains a negative weight cycle
			if (cost[v][v] < 0) {
				return;
			}
		}
	}
}

// Recurive Function to print path of given vertex u from source vertex v
void CFloydWarshell::printPath(std::vector<std::vector<double>> &path, int v, int u, vector<int> &pathList)
{
	if (path[v][u] == v)
		return;

	printPath(path, v, path[v][u], pathList);
	pathList.push_back(path[v][u]);
}

// Function to print path information between all pairs of vertices
void CFloydWarshell::getConnectedPath(std::vector<std::vector<double>> &path, int startNode, int endNode, 
	vector<int> &pathList)
{
	if (startNode != endNode && path[startNode][endNode] != -1)
	{
		pathList.push_back(startNode);
		printPath(path, startNode, endNode, pathList);
		pathList.push_back(endNode);
	}
}
