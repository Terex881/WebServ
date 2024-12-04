
#pragma once
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

    public:
        Request(string body);

};
// add const 
