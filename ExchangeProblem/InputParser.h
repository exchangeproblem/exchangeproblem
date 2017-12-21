

#include "stdafx.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>

using namespace std;

enum InputRequestType
{
	EXCHANGE_RATE_REQUEST,
	PRICE_UPDATE_REQUEST,
	DISPLAY_GRAPH,
	EXIT,
	INVALID_REQUEST
};

struct PriceUpdateData
{
	string timestamp;
	string exchange;
	string source_currency;
	string dest_currency;
	string forward_fector;
	string backward_fector;
};

struct ExchangeReqData
{
	string source_exchange;
	string source_currency;
	string dest_exchange;
	string dest_currency;
};

class CInputParser
{
public:
	InputRequestType GetInputType(string line);
	void ParsePriceUpdate(string line, PriceUpdateData &data);
	void ParseExchangeReqData(string line, ExchangeReqData &data);

};