/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:52 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/11 20:48:21 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Body.hpp"
#include "Header.hpp"

Request::Request()
{
	body_obj = NULL;
	header_obj = NULL;
	clientData.bodySize = 0;
	clientData.requestStat = 0;
}

Request::~Request()
{
	delete body_obj; body_obj = NULL;
	delete header_obj; header_obj = NULL;
}

Body* Request::getBody()
{
	if (!body_obj)
		body_obj = new Body();
	return body_obj;
}

Header* Request::getHeader()
{	
	if (!header_obj)
		header_obj = new Header();
	return header_obj;
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
	const map<string, string>& headers = header_obj->getMap();
	map<string, string>::const_iterator it = headers.find(element);
	
	if (it != header_obj->getMap().end())
	{
		res = it->second;		
	}
	return res; // return an error 
}


void Request::request(string &request)
{

	getBody();
	getHeader();



	body_obj->setAttay(this);
	header_obj->setAttay(this);

	

	if (!clientData.requestStat)
	{
		
		size_t pos = request.find(DCRLF);
		if (pos != string::npos)
		{
			clientData.header.append(request.c_str(), 0, pos);
			header_obj->parseHeader(clientData.header);
			request.erase(0, pos + 4);
		}
		else
			clientData.header.append(request);
	}
	if (clientData.requestStat == 1)
	{
		if (clientData.requestMethod == "POST")
			body_obj->parseBodyTypes(request);
		else if (clientData.requestMethod == "GET")
		{
			clientData.requestStat = 2;
		}
		// else if (clientData.requestMethod == "DELETE");
			//
	}
}