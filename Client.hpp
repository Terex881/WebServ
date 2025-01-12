
#pragma once 



#include "Response.hpp"
#include "Method/Request/Request.hpp"


class Client
{
	public:
		Request		req_obj;
		Response	res_obj;
	Response&	getRes()
	{
		return res_obj;
	}
	Request&	getReq()
	{
		return req_obj;
	}



};