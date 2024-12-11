#pragma once
#include <cstddef>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <fstream>

using namespace std;

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

// void parseRequest(string str);


class Request
{
    private:
        std::map<string, string> mp;
        size_t bodySize;
        static int isFinish;
    	static string header;
    	static string body;


    public:
        Request();
        void request(string &body);
        void parseBodyTypes(string line, map<string, string> headerMap);
        void parseHeader(string &header);
        void print(map<string, string> &headerMap);



};
// add const 
