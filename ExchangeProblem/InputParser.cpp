
#include "stdafx.h"
#include "InputParser.h"

InputRequestType CInputParser::GetInputType(string line)
{
	if (line.length() == 0)
		return INVALID_REQUEST;
	
	if (line.find("EXCHANGE_RATE_REQUEST") == 0)
		return EXCHANGE_RATE_REQUEST;
	else if(line.find("DISPLAY_GRAPH") == 0)
		return DISPLAY_GRAPH; //this is only for debugging
	else if (line.find("EXIT") == 0)
		return EXIT; 
	else {
		size_t spacecount = std::count(line.begin(), line.end(), ' ');
		if (spacecount == 5 || spacecount == 6) //assuming last character also space
			return PRICE_UPDATE_REQUEST;
		else
			return INVALID_REQUEST;
	}
}

void CInputParser::ParseExchangeReqData(string line, ExchangeReqData &data)
{
	int cnt = 0;
	int lastpos = 0;

	int currPos = line.find(' ', lastpos);
	while (currPos != -1)
	{
		string szData = line.substr(lastpos, currPos - lastpos);
		switch (cnt)
		{
		case 0:
			break;
		case 1:
			data.source_exchange = szData;
			break;
		case 2:
			data.source_currency = szData;
			break;
		case 3:
			data.dest_exchange = szData;
			break;
		case 4:
			data.dest_currency = szData;
			break;
		}
		cnt++;

		lastpos = currPos + 1;
		currPos = line.find(' ', lastpos);
	}

	if (cnt == 4){
		data.dest_currency = line.substr(lastpos);
	}
}

void CInputParser::ParsePriceUpdate(string line, PriceUpdateData &data)
{
	int cnt = 0;
	int lastpos = 0;

	int currPos = line.find(' ', lastpos);
	while (currPos != -1)
	{
		string szData = line.substr(lastpos, currPos - lastpos);
		switch (cnt)
		{
		case 0:
			data.timestamp = szData;
			break;
		case 1:
			data.exchange = szData;
			break;
		case 2:
			data.source_currency = szData;
			break;
		case 3:
			data.dest_currency = szData;
			break;
		case 4:
			data.forward_fector = szData;
			break;
		case 5:
			data.backward_fector = szData;
			break;
		}
		cnt++;

		lastpos = currPos + 1;
		currPos = line.find(' ', lastpos);
	}

	if (cnt == 5){
		data.backward_fector = line.substr(lastpos);
	}
}
