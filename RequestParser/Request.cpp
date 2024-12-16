#include "./Request.hpp"

int Request::isFinish = 0;
string Request::header;
string Request::body;

Request::Request()
{
	bodySize = 0;
	isFinish = 0;
	boundry = "";
	endBoundry = "";
}


Request::~Request()
{
	// if (outFile.is_open())
		// outFile.close();
}

void Request::print(map<string, string> &mp)
{
	for(map<string, string>::iterator it = mp.begin(); it!=mp.end(); it++)
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":" << endl;
}


void Request::request(string &request)
{
	if (!isFinish)
	{
		size_t pos = request.find("\r\n\r\n");
		if (pos != string::npos)
		{
			header += request.substr(0, pos);
			request.erase(0, pos + 4);
			parseHeader(header);
		}
		else
			header.append(request);
	}
	if (isFinish == 1)
	{
		body.append(request);
		parseBodyTypes(request, mp);
	}
}