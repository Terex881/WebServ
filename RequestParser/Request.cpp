#include "./Request.hpp"

string	Request::header;
// string	Request::body;
int		Request::FINISHED			= 0;
int		Request::CHUNKED			= 0;
int		Request::CHUNKED_BOUNDARY	= 0;
int		Request::BOUNDARY			= 0;
int		Request::CONTENT_LENGTH		= 0;
int		Request::REQUEST_FINISH		= 0;

Request::Request()
{
	bodySize = 0;
	// FINISHED = 0;
	boundry = "";
	endBoundry = "";
	
}


Request::~Request()
{

}

void Request::fillData(std::map<string, string> mp)
{
	FINISHED = 1;

	map<string, string>::iterator lengthPos = mp.find("Content-Length");
	map<string, string>::iterator boundry = mp.find("Content-Type");
	map<string, string>::iterator chunked = mp.find("Transfer-Encoding");
	
	if (lengthPos != mp.end())
		bodySize = std::atol(lengthPos->second.c_str());
	if(mp.find("Host") == mp.end())
		cout << RED << "no Host found !!\n" << RESET;
	
	
	BOUNDARY = boundry != mp.end() && boundry->second.find("multipart/form-data;") != std::string::npos;
	if(BOUNDARY)
	{
		string sep = boundry->second.substr(boundry->second.find("=") + 1,boundry->second.find("\r") - boundry->second.find("=") - 1);
		this->boundry = "\r\n--" + sep + "\r\n";
		this->endBoundry = "\r\n--" + sep + "--\r\n";
	}

	if (chunked != mp.end())
	{
		if (chunked->second != "chunked")
			cout << RED << "not implemented\n" ;
		else if (chunked->second == "chunked" && !BOUNDARY)
			CHUNKED = 1;
		else if (chunked->second == "chunked" && BOUNDARY)
			CHUNKED_BOUNDARY = 1;
	}
}


void Request::print(map<string, string> &mp)
{
	for(map<string, string>::iterator it = mp.begin(); it!=mp.end(); it++)
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":" << endl;
}


void Request::request(string &request)
{
	
	if (!FINISHED)
	{
		size_t pos = request.find("\r\n\r\n");
		if (pos != string::npos)
		{
			header += request.substr(0, pos);
			request.erase(0, pos + 2);
			parseHeader(header);
		}
		else
			header.append(request);
	}
	if (FINISHED)
	{
		// body.append(request);
		
		parseBodyTypes(request);
	}
}