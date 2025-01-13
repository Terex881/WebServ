/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:52 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/13 09:39:30 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
	BodyData.bodySize = 0;
	RequestData.requestStat = 0;
	BodyData.outFile = NULL;	
}

Request::~Request()	{}



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
	if (!BodyData.outFile)
		BodyData.outFile = new ofstream;
	if (!RequestData.requestStat)
	{		
		size_t pos = request.find(DCRLF);
		if (pos != string::npos)
		{
			RequestData.header.append(request.c_str(), 0, pos);
			parseHeader(RequestData.header);
			request.erase(0, pos + 4);
		}
		else
			RequestData.header.append(request);
	}
	if (RequestData.requestStat == 1)
	{
		if (HeaderData.requestMethod == "POST")
			parseBodyTypes(request);
		else if (HeaderData.requestMethod == "GET")
		{
			RequestData.requestStat = 2;
		}
		// else if (RequestData.requestMethod == "DELETE");
			//
	}
}