#include "./Request.hpp"
#include <cstddef>

unsigned long	Request::length				= 0;
unsigned long	Request::restLength			= 0;
int				Request::TYPE				= 0;
int				Request::REQUEST_IS_FINISH	= 0;

Request::Request()
{
	bodySize = 0;
	boundry = "";
	endBoundry = "";
}

Request::~Request()
{

}

int Request::getStat() const
{
	return REQUEST_IS_FINISH;
}

void Request::fillData(const std::map<string, string> &mp)
{
	REQUEST_IS_FINISH = 1;

	map<string, string>::const_iterator	lengthPos = mp.find("content-length");
	map<string, string>::const_iterator	boundry = mp.find("content-type");
	map<string, string>::const_iterator	chunked = mp.find("transfer-encoding");
	
	if (lengthPos != mp.end())
		bodySize = std::atol(lengthPos->second.c_str());
	if (mp.find("host") == mp.end())
		cout << RED << "no Host found !!\n" << RESET;
	
	
	bool bol = boundry != mp.end() && boundry->second.find("multipart/form-data;") != std::string::npos;
	if (bol)
	{
		string sep = boundry->second.substr(boundry->second.rfind("boundary=") + 9 , boundry->second.length());
		this->boundry		= "\r\n--" + sep + "\r\n";
		this->endBoundry	= "\r\n--" + sep + "--\r\n";
		TYPE = 0;
	}
	if (chunked != mp.end())
	{
		if (chunked->second != "chunked")
			cout << RED << "not implemented\n" << RESET ;
		else if (chunked->second == "chunked" && !bol)
			TYPE = 1;
		else if (chunked->second == "chunked" && bol)
			TYPE = 2;
	}
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