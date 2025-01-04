/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:52 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/04 11:18:00 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Body.hpp"
#include "Header.hpp"

Request::Request()
{
	data.body_obj = NULL;
	data.header_obj = NULL;
	data.bodySize = 0;
	data.requestStat = 0;
	// data.TYPE = 0;
}

Request::~Request()
{
	delete data.body_obj; data.body_obj = NULL;
	delete data.header_obj; data.header_obj = NULL;
}

Body* Request::getBody()
{
	if (!data.body_obj)
		data.body_obj = new Body();
	return data.body_obj;
}

Header* Request::getHeader()
{
	if (!data.header_obj)
		data.header_obj = new Header();
	return data.header_obj;
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

void Request::request(string &request)
{	
	data.body_obj->setAttay(this);
	data.header_obj->setAttay(this);

	if (!data.requestStat)
	{
		size_t pos = request.find(DCRLF);
		if (pos != string::npos)
		{
			data.header.append(request.c_str(), 0, pos);
			data.header_obj->parseHeader(data.header);
			request.erase(0, pos + 4);
		}
		else
			data.header.append(request);
	}
	if (data.requestStat == 1)
		data.body_obj->parseBodyTypes(request);
}