#include "Client.hpp"


Client::Client()    {}
Client::~Client()    {}

Response&	Client::getRes()
{
	return res_obj;
}

Request&	Client::getReq()
{
	return req_obj;
}

Cgi&	Client::getCgi()
{
	return cgi_obj;
}