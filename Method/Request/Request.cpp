/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:52 by sdemnati          #+#    #+#             */
/*   Updated: 2025/02/06 12:28:45 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
	BodyData.bodySize = 0;
	RequestData.requestStat = 0;
	RequestData.codeStatus = 200;
	RequestData.isDirListening = false;
	RequestData.isRedirect = false;
	RequestData.isCgi = false;
	RequestData.isUpload = false;
	HeaderData.isAlive = true;
	BodyData.bodyType = NONE;
	RequestData.errorFlag = true;
}

Request::~Request()	{}


void Request::clean(int code, string message)
{
	RequestData.errorFlag = false;
	RequestData.codeStatus = code;
	RequestData.codeStatusMap.insert(make_pair(code, message));
}


void Request::clearData()
{
	
	BodyData.bodySize = 0;
	RequestData.requestStat = 0;
	RequestData.codeStatus = 200;
	RequestData.isDirListening = false;
	RequestData.isRedirect = false;
	RequestData.isCgi = false;
	RequestData.isUpload = false;
	HeaderData.isAlive = true;
	BodyData.bodyType = NONE;
	RequestData.errorFlag = true;
	
	BodyData.boundry.clear();
	BodyData.endBoundry.clear();
	// BodyData.outFile.clear();
	BodyData.newStr.clear();
	BodyData.buffer.clear();
	BodyData.fileName.clear();
	BodyData.pathFormData.clear();

	HeaderData.port.clear();
	HeaderData.extension.clear();
	HeaderData.requestMethod.clear();
	HeaderData.queryStringVec.clear();
	HeaderData.bigMap.clear();
	HeaderData.url.clear();
	HeaderData.urlFinal.clear();

	RequestData.header.clear();
	RequestData.requestStat = 0;
	RequestData.first.clear();
	RequestData.serverName.clear();
	RequestData.cgiError.clear();
	RequestData.timeOut.clear();
	RequestData.pathInfo.clear();
	RequestData.fileLocation.clear();
	RequestData.executable_file.clear();
	RequestData.redirection.clear();
	RequestData.default_page.clear();
	RequestData.codeStatusMap.clear();
	RequestData.hostName.clear();

}


void Request::print1(std::map<string, string> &mp)
{
	for(map<string, string>::iterator it = mp.begin(); it!=mp.end(); it++)
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":" << RESET << endl;;
}
void Request::print(vector<string> &mp)
{
	for(vector<string>::iterator it = mp.begin(); it!=mp.end(); it++)
		cout << RED << ":" << *it << ":" << endl;
}

void Request::printV(vector<pair<string, string> > &mp)
{
	for(vector<pair<string, string> >::iterator it = mp.begin(); it!=mp.end(); it++)
	{
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":";
		cout << "\n--------------------------------------------\n";
	}
}

t_Request&	Request::getRequestData()	{ return RequestData; }
t_Header&	Request::getHeaderData()	{ return HeaderData; }
t_Body&		Request::getBodyData()		{ return BodyData; }


void Request::request(string &request)
{
	if (!RequestData.requestStat)
	{		
		size_t pos = request.find(DCRLF);
		if (pos != NP)
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
		{
			parseBodyTypes(request);
		}
		else if (HeaderData.requestMethod == "GET")
		{
			RequestData.requestStat = 2;
		}
		else if (HeaderData.requestMethod == "DELETE")
		{
			RequestData.requestStat = 2;
		}
			
	}
}


File_Parsing& Request::getConfigFileObj()
{
	return configFileObj;
}

Request::Request(const Request &src)
{
	*this = src;
}

Request& Request::operator=(const Request &copy)
{
	if (this != &copy)
	{
		// BodyData.bodyType = copy.BodyData.bodyType;
		// BodyData.bodySize = copy.BodyData.bodySize;
		// BodyData.boundry = copy.BodyData.boundry;
		// BodyData.endBoundry = copy.BodyData.endBoundry;
		// BodyData.newStr = copy.BodyData.newStr;
		// BodyData.buffer = copy.BodyData.buffer;
		// BodyData.outFile = copy.BodyData.outFile;
		BodyData = copy.BodyData;
		RequestData = copy.RequestData;
		HeaderData = copy.HeaderData;
	}
	return *this;
}