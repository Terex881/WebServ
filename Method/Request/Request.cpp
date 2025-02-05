/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:52 by sdemnati          #+#    #+#             */
/*   Updated: 2025/02/05 10:27:10 by sdemnati         ###   ########.fr       */
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
}

Request::~Request()	{}


void Request::clean(int code, string message)
{
	RequestData.codeStatus = code;
	RequestData.codeStatusMap.insert(make_pair(code, message));
	RequestData.requestStat = 2;
	// throw runtime_error(message);
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
	HeaderData.requestMethod.clear();
	HeaderData.bigMap.clear();
	HeaderData.queryStringVec.clear();
	// HeaderData.url.clear();

	RequestData.header.clear();
	// RequestData.requestStat.clear();
	// RequestData.fd.clear();
	// RequestData.is_server.clear();
	// RequestData.is_client.clear();
	// RequestData.sent_head.clear();
	RequestData.first.clear();
	// RequestData.bytes_sent.clear();
	// RequestData.file.clear();
	// RequestData.codeStatus.clear();
	// RequestData.isDirListening.clear();
	// RequestData.isRedirect.clear();
	// RequestData.maxBodySize.clear();
	RequestData.serverName.clear();
	// RequestData.isCgi.clear();
	RequestData.timeOut.clear();
	RequestData.pathInfo.clear();
	// RequestData.extension.clear();
	RequestData.fileLocation.clear();
	RequestData.executable_file.clear();
	RequestData.redirection.clear();
	RequestData.default_page.clear();
	// RequestData.isUpload.clear();

	// BodyData.bodyType = NONE;
	// BodyData.bodySize.clear();
	BodyData.boundry.clear();
	BodyData.endBoundry.clear();
	BodyData.outFile.clear();
	BodyData.newStr.clear();
	BodyData.buffer.clear()	;
	BodyData.fileName.clear();
	
	RequestData.codeStatusMap.clear();
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