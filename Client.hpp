
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


enum type
{
	BOUNDARY,
	CHUNKED,
	CHUNKED_BOUNDARY,
	BODY_SIZE,
	NONE,
};

typedef struct s_Data
{
	std::map<string, string>	queryStringMap;
	string						boundry;
	string						endBoundry;
	string						extention;
	string						header;
	size_t						bodySize;
	type						bodyType;
	int							requestStat;
	string						requestMethod;
	//------------------------------------------------------
	int fd;
	// struct sockaddr_in addr;
	int	is_server;
	int	is_client;
	int sent_head;
	string	url;
	string	first;
	// Response rsp;
	size_t	bytes_sent;
	std::ifstream *file;
	
}	t_Data;

class Client
{
	public:
		t_Data clientData;
		Request *req_obj;
		Response *res_obj;

	Client() : req_obj(NULL), res_obj(NULL)
	{}



};