/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:52 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/09 15:53:43 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Body.hpp"
#include "Header.hpp"

Request::Request()
{
	requestData.body_obj = NULL;
	requestData.header_obj = NULL;
	requestData.bodySize = 0;
	requestData.requestStat = 0;
}

Request::~Request()
{
	delete requestData.body_obj; requestData.body_obj = NULL;
	delete requestData.header_obj; requestData.header_obj = NULL;
}

Body* Request::getBody()
{
	if (!requestData.body_obj)
		requestData.body_obj = new Body();
	return requestData.body_obj;
}

Header* Request::getHeader()
{	
	if (!requestData.header_obj)
		requestData.header_obj = new Header();
	return requestData.header_obj;
}

void Request::print(map<string, string> &mp)
{
	for(map<string, string>::iterator it = mp.begin(); it!=mp.end(); it++)
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":" << endl;
}

void Request::printV(vector<pair<string, string> > &mp)
{
	for(vector<pair<string, string> >::iterator it = mp.begin(); it!=mp.end(); it++)
	{
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":";
		cout << "\n--------------------------------------------\n";
	}
}

string Request::getElement(const string & element)
{
	string res;
	const map<string, string>& headers = requestData.header_obj->getMap();
	map<string, string>::const_iterator it = headers.find(element);
	
	if (it != requestData.header_obj->getMap().end())
	{
		res = it->second;		
	}
	return res; // return an error 
}


void Request::request(string &request)
{

	getBody();
	getHeader();



	requestData.body_obj->setAttay(this);
	requestData.header_obj->setAttay(this);

	

	if (!requestData.requestStat)
	{
		
		size_t pos = request.find(DCRLF);
		if (pos != string::npos)
		{
			requestData.header.append(request.c_str(), 0, pos);
			requestData.header_obj->parseHeader(requestData.header);
			request.erase(0, pos + 4);
		}
		else
			requestData.header.append(request);
	}
	if (requestData.requestStat == 1)
	{
		if (requestData.requestMethod == "POST")
			requestData.body_obj->parseBodyTypes(request);
		else if (requestData.requestMethod == "GET")
		{
			requestData.requestStat = 2;
		}
		// else if (requestData.requestMethod == "DELETE");
			//
	}
}