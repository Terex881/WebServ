/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:52 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/12 18:55:42 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
	BodyData.bodySize = 0;
	RequestData.requestStat = 0;
	BodyData.outFile = new ofstream;

}

Request::~Request()
{
	delete BodyData.outFile;
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
	map<string, string>::const_iterator it = HeaderData.bigMap.find(element);
	
	if (it != HeaderData.bigMap.end())
	{
		res = it->second;		
	}
	return res;
}


void Request::request(string &request)
{
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