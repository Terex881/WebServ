#pragma once
#include <cstddef>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <fstream>

using namespace std;

void parseRequest(string str);


class Request
{
    private:
    	std::map<string, string> headerMap;
       static  size_t size1;

    public:
        Request(string body);
        void parseBodyTypes(string line, map<string, string> headerMap);


};
// add const 
