#include "Request.hpp"
#include "Body.hpp"
#include "Header.hpp"

// string									Request::boundry;
// string									Request::endBoundry;
// string 									Request::extention;
// size_t									Request::bodySize = 0;
// int										Request::TYPE = 0;
// int										Request::REQUEST_IS_FINISH = 0;
// string									Request::header;


Request::Request() : body_obj(getBody()), header_obj(getHeader())
{
	bodySize = 0;
	REQUEST_IS_FINISH = 0;
	TYPE = 0;
	cout << YELLOW << "OKKK\n" << RESET << endl;
}

Request::~Request()
{
	delete body_obj;
	delete header_obj;
}

Body* Request::getBody()
{
	if (!body_obj)
		body_obj = new Body();
	return body_obj;
}

Header* Request::getHeader()
{
	if (!header_obj)
		header_obj = new Header();
	return header_obj;
}



int Request::getStat() const
{
	return REQUEST_IS_FINISH;
}

int Request::getType() const
{
	return TYPE;
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


void Request::request(string &request)
{
	header_obj = getHeader();
	body_obj = getBody();
	cout << header_obj << endl;
	exit(2);
	if (!getStat())
	{
		size_t pos = request.find(DCRLF);
		if (pos != string::npos)
		{
			header.append(request.c_str(), 0, pos);
			header_obj->parseHeader(header, this);
			request.erase(0, pos + 4);
		}
		else
			header.append(request);
	}
	if (getStat() == 1)
	{
		body_obj->parseBodyTypes(request, this);
	}
}