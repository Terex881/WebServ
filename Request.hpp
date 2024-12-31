#pragma once

#include <cstddef>
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
		string getB() const;
		string getEndB() const;
		string getEx() const;
		size_t getSize() const;
		
		void setStat(int _stat);
		void setType(int _stat);
		void setB(string _B) ;
		void setEndB(string _Eb) ;
		void setEx(string _Ex) ;
		void setSize(size_t _Size) ;
		


		void print(map<string, string> &headerMap);
		void printV(vector<pair<string, string> > &mp);

		Body* getBody();
		Header* getHeader();
};

#include "Body.hpp"
#include "Header.hpp"
