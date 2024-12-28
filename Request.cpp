#include "Request.hpp"

Request::Request()
{
	bodySize = 0;
	// boundry = "";
	// endBoundry = "";
	REQUEST_IS_FINISH = 0;
}



int Request::getStat() const
{
	return REQUEST_IS_FINISH;
}



void Request::print(map<string, string> &mp)
{
	for(map<string, string>::iterator it = mp.begin(); it!=mp.end(); it++)
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":" << endl;
}

void Request::printV(vector<pair<string, string> > &mp)
{
	for(vector<pair<string, string> >::iterator it = mp.begin(); it!=mp.end(); it++)
	{
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":" << endl;
		cout << "\n--------------------------------------------\n";

	}
}

void	Request::parseBodyTypes(string &body)
{
	// newStr += body;
	newStr.append(body.c_str(), body.length());

	switch (TYPE)
	{
		case (0):parseBoundryBody(newStr); break;
		case (1): parseChunkedBody(newStr); break;
		case (2): parseChunkedBoundryBody(newStr); break;
		case (3): parseBodyLength(newStr); break;
	}
}

void Request::request(string &request)
{
	if (!getStat())
	{
		size_t pos = request.find("\r\n\r\n");
		if (pos != string::npos)
		{
			// header += request.substr(0, pos); // chof kifach
			header.append(request.c_str(), 0, pos);
			parseHeader(header);
			request.erase(0, pos + 4);
		}
		else
			header.append(request);
	}
	if (getStat() == 1)
		parseBodyTypes(request);
}
// 6.44.30
// 6.49.20 --> for 2.40 GO

// 12.18.20
// 