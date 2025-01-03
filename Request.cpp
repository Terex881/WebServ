/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:52 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/03 18:29:21 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Body.hpp"
#include "Header.hpp"

Request::Request()
{
	d.body_obj = NULL;
	d.header_obj = NULL;
	d.bodySize = 0;
	d.REQUEST_IS_FINISH = 0;
	d.TYPE = 0;
}

Request::~Request()
{
	delete d.body_obj; d.body_obj = NULL;
	delete d.header_obj; d.header_obj = NULL;
}

Body* Request::getBody()
{
	if (!d.body_obj)
		d.body_obj = new Body();
	return d.body_obj;
}

Header* Request::getHeader()
{
	if (!d.header_obj)
		d.header_obj = new Header();
	return d.header_obj;
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
	d.body_obj->setAttay(this);
	d.header_obj->setAttay(this);

	if (!d.REQUEST_IS_FINISH)
	{
		size_t pos = request.find(DCRLF);
		if (pos != string::npos)
		{
			d.header.append(request.c_str(), 0, pos);
			d.header_obj->parseHeader(d.header);
			request.erase(0, pos + 4);
		}
		else
			d.header.append(request);
	}
	if (d.REQUEST_IS_FINISH == 1)
		d.body_obj->parseBodyTypes(request);
}

// void Request::setStat(const int &stat)
// {
// 	REQUEST_IS_FINISH = stat;
// }
// void Request::setType(const int &stat)
// {
// 	TYPE = stat;
// }
// void Request::setB(const string &_B)
// {
// 	boundry = _B;
// }
// void Request::setEndB(const string &_B)
// {
// 	endBoundry = _B;
// }
// void Request::setEx(const string &_Ex)
// {
// 	extention = _Ex;
// }
// void Request::setSize(const size_t &_s)
// {
// 	bodySize = _s;
// }

// string Request:: getB() const
// {
// 	return boundry;
// }
// string Request:: getEndB() const
// {
// 	return endBoundry;
// }
// string Request:: getEx() const
// {
// 	return extention;
// }
// size_t Request:: getSize() const
// {
// 	return bodySize;
// }
// int Request::getStat() const
// {
// 	return REQUEST_IS_FINISH;
// }

// int Request::getType() const
// {
// 	return TYPE;
// }