#pragma once

#include "../Response.hpp"
#include "../Method/Request/Request.hpp"
#include "../cgi-bin/Cgi.hpp"

class Client
{
	private:
		Request		req_obj;
		Response	res_obj;
		Cgi			cgi_obj;
	public:
		Client();
		~Client();
		
		struct kevent *event;
		Response&	getRes();
		Request&	getReq();
		Cgi&	getCgi();
};