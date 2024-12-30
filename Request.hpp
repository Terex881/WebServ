#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>

#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define FILE_NAME "; filename=\""

using namespace std;


class Header;
class Body;

class Request
{
	protected:
		string					boundry;
		string					endBoundry;
		string					extention;
		size_t					bodySize;
		int						TYPE;
		int						REQUEST_IS_FINISH;
		string					header;

		Body* body_obj;
		Header* header_obj;

	public:

		Request();
		~Request();

		void request(string &body);
		int getStat() const;
		int getType() const;
		void setStat(int stat)
		{
			REQUEST_IS_FINISH = stat;
		}
		void setType(int stat)
		{
			TYPE = stat;
		}
		void print(map<string, string> &headerMap);
		void printV(vector<pair<string, string> > &mp);

		Body* getBody();
		Header* getHeader();
};

#include "Body.hpp"
#include "Header.hpp"
