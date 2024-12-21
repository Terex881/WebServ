#include "Request.hpp"

unsigned long	Request::length				= 0;
int				Request::TYPE				= 0;
int				Request::REQUEST_IS_FINISH	= 0;

Request::Request()
{
	bodySize = 0;
	boundry = "";
	endBoundry = "";
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
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":" << endl;
}

void Request::request(string &request)
{
	if (!REQUEST_IS_FINISH)
	{
		size_t pos = request.find("\r\n\r\n");
		if (pos != string::npos)
		{
			header += request.substr(0, pos); // chof kifach
			parseHeader(header);
			if (!TYPE)
				request.erase(0, pos + 2);
			else
				request.erase(0, pos + 4);

		}
		else
			header.append(request);
	}
	if (REQUEST_IS_FINISH == 1)
		parseBodyTypes(request);
}