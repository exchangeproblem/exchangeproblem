// ExchangeProblem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FloydWarshell.h"
#include "InputParser.h"
#include <map>

#define INITIAL_VECTOR_SIZE 5000
#define VECTOR_GROW_SIZE	250

//Vertex  Name = (Exchange, Currency)
std::map<int, string> IndexToVertex;
std::map<string, int> VertexToIndex;

// Function to print the shortest cost
void printCostMatrix(std::vector<std::vector<double>> &cost, int NumofVertices)
{
	std::cout << "\nCost Matrix is : \n";
	for (int v = 0; v < NumofVertices; v++)
	{
		for (int u = 0; u < NumofVertices; u++)
		{
			if (cost[v][u] == DBL_MAX)
				std::cout << setw(5) << "INF";
			else
				std::cout << setw(5) << cost[v][u];
		}
		std::cout << endl;
	}

	std::cout << endl;
}

// Function to print pth for the shortest cost
void printConnectedPath(std::vector<std::vector<double>> &path, int NumofVertices, CFloydWarshell &objFloydWarshell)
{
	for (int v = 0; v < NumofVertices; v++)
	{
		for (int u = 0; u < NumofVertices; u++)
		{
			if (v != u && path[v][u] != -1)
			{
				//Get Path by trversing path array
				vector<int> pathList;
				objFloydWarshell.getConnectedPath(path, v, u, pathList);

				//print vector
				std::cout << "Shortest Path from vertex " << v << " to vertex " << u << " is ( ";
				for (std::vector<int>::iterator it = pathList.begin(); it != pathList.end(); ++it)
					std::cout << *it << " ";
				std::cout << ">" << endl;
			}
		}
	}
}


//Function to Print Adj_Matrix Graph
void printAdjMatrixGrph(std::vector<std::vector<double>> &Adj_Matrix, int NumofVertices)
{
	std::cout << "Vertex Graph is" << endl;
	for (int v = 0; v < NumofVertices; v++)
	{
		for (int u = 0; u < NumofVertices; u++)
		{
			if (Adj_Matrix[v][u] != DBL_MAX && v != u)
				std::cout << IndexToVertex[v] << " -- " << Adj_Matrix[v][u] << " --> " << IndexToVertex[u] << endl;
		}
	}
}

void ResizeVector(std::vector<std::vector<double>> &Adj_Matrix, std::vector<std::vector<double>> &cost, 
	std::vector<std::vector<double>> &path)
{
	int currCapability = Adj_Matrix.capacity();
	currCapability += VECTOR_GROW_SIZE;

	Adj_Matrix.resize(currCapability);
	cost.resize(currCapability);
	path.resize(currCapability);

	for (int i = 0; i < currCapability; i++){
		Adj_Matrix[i].resize(currCapability, DBL_MAX);
		cost[i].resize(currCapability, 0);
		path[i].resize(currCapability, 0);
	}
	
}

void UpdateNewExchangeRate(PriceUpdateData &data, int &NumofVertices,
	std::vector<std::vector<double>> &Adj_Matrix, std::vector<std::vector<double>> &cost, 
	std::vector<std::vector<double>> &path)
{
	string srcVertex = "<" + data.exchange + ", " + data.source_currency + ">";
	string destVertex = "<" + data.exchange + ", " + data.dest_currency + ">";
	
	int srcIndex, destIndex;

	bool isSrcVertexExist = true;
	std::map<string, int>::iterator itSrc = VertexToIndex.find(srcVertex);
	if (itSrc == VertexToIndex.end()) 
	{
	
		isSrcVertexExist = false;

		//Check if needs to resize vector...
		if (Adj_Matrix.capacity() == VertexToIndex.size()) {
			ResizeVector(Adj_Matrix, cost, path);
		}

		//Add Vertex to list
		VertexToIndex[srcVertex] = NumofVertices;
		IndexToVertex[NumofVertices] = srcVertex;		
		srcIndex = NumofVertices;
		Adj_Matrix[srcIndex][srcIndex] = 0;
		NumofVertices++;
	}
	else
	{
		srcIndex = itSrc->second;
	}

	bool isDestVertexExist = true;
	std::map<string, int>::iterator itDest = VertexToIndex.find(destVertex);
	if (itDest == VertexToIndex.end())
	{
		isDestVertexExist = false;

		//Check if needs to resize vector...
		if (Adj_Matrix.capacity() == VertexToIndex.size()) {
			ResizeVector(Adj_Matrix, cost, path);
		}

		//Add Vertex to list
		VertexToIndex[destVertex] = NumofVertices;
		IndexToVertex[NumofVertices] = destVertex;
		destIndex = NumofVertices;
		Adj_Matrix[destIndex][destIndex] = 0;		
		NumofVertices++;
	}
	else
	{
		destIndex = itDest->second;
	}

	Adj_Matrix[srcIndex][destIndex] = std::stod(data.forward_fector);
	Adj_Matrix[destIndex][srcIndex] = std::stod(data.backward_fector);

	//Rule 2 : For each currency, each (exchange, currency) is connected 
	//to every other (other_exchange, currency) with an edge weight of 1.0
	if (isSrcVertexExist == false || isDestVertexExist == false)
	{
		string szExchange = "<" + data.exchange;
		string szSrcCurrency = data.source_currency + ">";
		string szDestCurrency = data.dest_currency + ">";

		for (std::map<string, int>::iterator it = VertexToIndex.begin(); it != VertexToIndex.end(); ++it)
		{
			string vertex = it->first;
			if (vertex.find(szExchange) == -1) //check if vertex from other exchange..
			{
				//check if source currency found for other exchange..
				if (isSrcVertexExist == false && vertex.find(szSrcCurrency) != -1) {
					int Index = it->second;
					Adj_Matrix[srcIndex][Index] = 1;
					Adj_Matrix[Index][srcIndex] = 1;
				}

				//check if same currency found for other exchange..
				if (isDestVertexExist == false && vertex.find(szDestCurrency) != -1) {
					int Index = it->second;
					Adj_Matrix[destIndex][Index] = 1;
					Adj_Matrix[Index][destIndex] = 1;
				}
			}
		}
	}	
}

//assuming first line in above sample output is <rate> <= Minimum cost between 2 vertex...
void PrintBestRatePath(ExchangeReqData &data, std::vector<std::vector<double>> &cost, 
	std::vector<std::vector<double>> &path, CFloydWarshell &objFloydWarshell)
{
	string szSrcVertex = "<" + data.source_exchange + ", " + data.source_currency + ">";
	string szDestVertex = "<" + data.dest_exchange + ", " + data.dest_currency + ">";
		
	std::map<string, int>::iterator itSrc = VertexToIndex.find(szSrcVertex);
	if (itSrc == VertexToIndex.end()) {
		std::cout << "Source Vertex " + szSrcVertex + " not exist...\n";
		return;
	}

	std::map<string, int>::iterator itDest = VertexToIndex.find(szDestVertex);
	if (itDest == VertexToIndex.end())
	{
		std::cout << "Destination Vertex " + szDestVertex + " not exist...\n";
		return;
	}

	int srcVertex = itSrc->second;
	int destVertex = itDest->second;
	
	string szCost;
	if (cost[srcVertex][destVertex] == DBL_MAX)
		szCost = "INF";
	else
		szCost = std::to_string(cost[srcVertex][destVertex]);

	std::cout << "BEST_RATES_BEGIN " + data.source_exchange + " " + data.source_currency + " " + data.dest_exchange + " " + data.dest_currency + " " + szCost  + "\n";
	
	vector<int> pathList;
	objFloydWarshell.getConnectedPath(path, srcVertex, destVertex, pathList);

	//print path vector
	for (std::vector<int>::iterator it = pathList.begin(); it != pathList.end(); ++it)
		std::cout << IndexToVertex[*it] << "\n";

	std::cout << "BEST_RATES_END\n";
}

//<timestamp> <exchange> <source_currency> <destination_currency> <forward_factor> <backward_factor>
//2017-11-01T09:42:23+00:00 KRAKEN BTC USD 1000.0 0.0009
//EXCHANGE_RATE_REQUEST <source_exchange> <source_currency> <destination_exchange> <destination_currency>
/*
2017-11-01T09:42:23+00:00 KRAKEN BTC USD 1000.0 0.0009
2017-11-01T09:42:23+00:00 KRAKEN BTC INR 5 7
2017-11-01T09:42:23+00:00 KRAKEN USD INR 6 3
2017-11-01T09:42:23+00:00 KRAKEN USD POUND 8 9
2017-11-01T09:42:23+00:00 NIFTY BTC USD 1000.0 0.0009
2017-11-01T09:42:23+00:00 NIFTY BTC INR 5 7
2017-11-01T09:42:23+00:00 NIFTY USD INR 6 3
2017-11-01T09:42:23+00:00 NIFTY USD POUND 8 9
2017-11-01T09:42:23+00:00 KRAKEN USD POUND 80 90
DISPLAY_GRAPH

EXCHANGE_RATE_REQUEST KRAKEN USD NIFTY USD
EXCHANGE_RATE_REQUEST KRAKEN USD NIFTY INR

BEST_RATES_BEGIN <source_exchange> <source_currency> <destination_exchange> <destination_currency> <rate>
<source_exchange, source_currency>
<exchange, currency>
<exchange, currency>
...
<destination_exchange, destination_currency>
BEST_RATES_END
*/

// main function
int _tmain(int argc, _TCHAR* argv[])
{	
//Allocate Initial memory to input the Adj_Matrix and cost[], path[] to stores shortest - path...
	std::vector<std::vector<double>> Adj_Matrix(INITIAL_VECTOR_SIZE, vector<double>(INITIAL_VECTOR_SIZE, DBL_MAX));
	std::vector<std::vector<double>> cost(INITIAL_VECTOR_SIZE, vector<double>(INITIAL_VECTOR_SIZE, 0));
	std::vector<std::vector<double>> path(INITIAL_VECTOR_SIZE, vector<double>(INITIAL_VECTOR_SIZE, 0));

	bool RateRecalculationReq = true;
	CFloydWarshell objFloydWarshell;
	int NumofVertices = 0;
	
	std::cout << "******Please Provide your input data********\n";

//Take input from stdin and do required...
	string line;
	InputRequestType reqType = INVALID_REQUEST;
	CInputParser inputParser;
	while (true && reqType != EXIT)
	{
		getline(cin, line);
		reqType = inputParser.GetInputType(line);
		switch (reqType)
		{
			case PRICE_UPDATE_REQUEST:
			{
				PriceUpdateData data;
				inputParser.ParsePriceUpdate(line, data);

				UpdateNewExchangeRate(data, NumofVertices, Adj_Matrix, cost, path);
				RateRecalculationReq = true; 
			}
			break;

			case EXCHANGE_RATE_REQUEST:
			{
				ExchangeReqData data;
				inputParser.ParseExchangeReqData(line, data);

				//for subsequent request no needs to recalculate cost matrix/path
				if (RateRecalculationReq == true) { 
					objFloydWarshell.FloydWarshell(Adj_Matrix, NumofVertices, cost, path);
					RateRecalculationReq = false;
				}

				PrintBestRatePath(data, cost, path, objFloydWarshell);
			}
			break;

			case DISPLAY_GRAPH: //this is only for debugging purpose
			{
				printAdjMatrixGrph(Adj_Matrix, NumofVertices);
			}
			break;

			case INVALID_REQUEST:
			{
				std::cout << "Invalid Request...\n";
			}
			break;
		}
	}

//Free the memory...
	for (unsigned int i = 0; i < Adj_Matrix.size(); i++){
		Adj_Matrix[i].empty();
		cost[i].empty();
		path[i].empty();
	}
	Adj_Matrix.empty();
	cost.empty();
	path.empty();
	
	return 0;
}
