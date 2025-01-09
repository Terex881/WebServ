
#pragma once 


#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "sstream"


#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define FILE_NAME "; filename=\""

using namespace std;

class Request;
class Response;

class Client
{
	public:
		Request *req_obj;
		Response *res_obj;

	Client() : req_obj(NULL), res_obj(NULL)
	{}



};