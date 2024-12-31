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


Request::Request() : body_obj(NULL), header_obj(NULL)
{
	bodySize = 0;
	REQUEST_IS_FINISH = 0;
	TYPE = 0;
}

Request::~Request()
{
	delete body_obj; body_obj = NULL;
	delete header_obj; header_obj = NULL;
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
	body_obj->atay_tkhwa = this;
	header_obj->ataty = this;

	if (!getStat())
	{
		size_t pos = request.find(DCRLF);
		if (pos != string::npos)
		{
			header.append(request.c_str(), 0, pos);
			header_obj->parseHeader(header);
			request.erase(0, pos + 4);
		}
		else
			header.append(request);
	}
	if (getStat() == 1)
	{
		body_obj->parseBodyTypes(request);
	}
}



void Request::setStat(const int &stat)
{
	REQUEST_IS_FINISH = stat;
}
void Request::setType(const int &stat)
{
	TYPE = stat;
}
void Request::setB(const string &_B)
{
	boundry = _B;
}
void Request::setEndB(const string &_B)
{
	endBoundry = _B;
}
void Request::setEx(const string &_Ex)
{
	extention = _Ex;
}
void Request::setSize(const size_t &_s)
{
	bodySize = _s;
}

string Request:: getB() const
{
	return boundry;
}
string Request:: getEndB() const
{
	return endBoundry;
}
string Request:: getEx() const
{
	return extention;
}
size_t Request:: getSize() const
{
	return bodySize;
}
int Request::getStat() const
{
	return REQUEST_IS_FINISH;
}

int Request::getType() const
{
	return TYPE;
}