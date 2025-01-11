
#pragma once 



#include "Response.hpp"
#include "Method/Request/Request.hpp"

// enum type
// {
// 	BOUNDARY,
// 	CHUNKED,
// 	CHUNKED_BOUNDARY,
// 	BODY_SIZE,
// 	NONE,
// };

// typedef struct s_Data
// {
// 	std::map<string, string>	queryStringMap;
// 	string						boundry;
// 	string						endBoundry;
// 	string						extention;
// 	string						header;
// 	size_t						bodySize;
// 	type						bodyType;
// 	int							requestStat;
// 	string						requestMethod;
// 	//------------------------------------------------------
// 	int fd;
// 	// struct sockaddr_in addr;
// 	int	is_server;
// 	int	is_client;
// 	int sent_head;
// 	string	url;
// 	string	first;
// 	// Response rsp;
// 	size_t	bytes_sent;
// 	std::ifstream *file;
	
// }	t_Data;
// 		t_Data clientData;

class Client
{
	public:
		Request		req_obj;
		Response	res_obj;

	Client() //: req_obj(NULL), res_obj(NULL)
	{}

	Response&	getRes()
	{
		return res_obj;
	}
	Request&	getReq()
	{
		return req_obj;
	}



};