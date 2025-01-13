/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:45 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/13 09:34:58 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"



void Request::storeQueryString(string &str, size_t QMPos)
{
	string	key, value;
	size_t	andPos,	endPos;
	string	queryStrings;

	queryStrings = str.substr(QMPos + 1, str.length());
	str.erase(QMPos);
	
	while (!queryStrings.empty()) 
	{
		andPos = queryStrings.find("&");
		endPos = (andPos != string::npos) ? andPos : queryStrings.length();

		size_t equalPos = queryStrings.find("=");
		if (equalPos != string::npos && equalPos < endPos) 
		{
			key = queryStrings.substr(0, equalPos);
			value = queryStrings.substr(equalPos + 1, endPos - equalPos - 1);
			HeaderData.queryStringMap.insert(make_pair(key, value));
		}
		if (andPos != string::npos)	queryStrings.erase(0, endPos + 1);
		else	queryStrings.clear();
	}
	// print(HeaderData.queryStringMap);
}
 
void Request::parseUri(string &str)
{
	if (str.find_first_not_of("%!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
		cout << RED << "bad URL" << std::endl;

	/* encoding reserved characters remoce % and 2 hex and replace it with character*/
	for(size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '%' && i + 2 < str.length() && isxdigit(str[i+1]) && isxdigit(str[i+2]))
		{
			char number = strtol(str.substr(i + 1, 2).c_str(), NULL, 16);
			str[i] = number;
			str.erase(i + 1, 2);
		}
		else if (str[i] == '+')
			str[i] = ' ';
	}
	// cout << BLUE << str << RESET << endl;

	/* search for is there any query strigs*/
	size_t QMPos = str.find("?");
	if (QMPos != string::npos)
		storeQueryString(str, QMPos);
	HeaderData.url = str;
}

void Request::parseFirstLine(string &line)
{
	string method, uri, httpVersion;
	map<string, string>::iterator it;

	if (isspace(line[0]))
		cout << RED << "foud space in the begening\n";

	std::istringstream ss(line);
	ss >> method >> uri >> httpVersion;
	
	HeaderData.bigMap.insert(std::make_pair("method", method));
	HeaderData.bigMap.insert(std::make_pair("uri", uri));
	HeaderData.bigMap.insert(std::make_pair("httpVersion", httpVersion));

	// cout << RED << uri << RESET << endl;
	HeaderData.requestMethod = method;
	if (HeaderData.requestMethod != "POST" && HeaderData.requestMethod != "GET" && HeaderData.requestMethod != "DELETE")
		cout << RED << "501 Not Implemented" << std::endl;
	if ((it = HeaderData.bigMap.find("uri")) != HeaderData.bigMap.end())
		parseUri(it->second);
	if ((it = HeaderData.bigMap.find("httpVersion")) != HeaderData.bigMap.end() && it->second != "HTTP/1.1")
		cout << RED << "error: invalid version" << std::endl;
}

void Request::parseHeader(string &header)
{
	string key, value;
	size_t colonPos, lineEnd;
	size_t pos = header.find(CRLF);
	string firstLine = header.substr(0, pos);

	// check if first line has space in start and print error 
	parseFirstLine(firstLine);
	header.erase(0, pos + 2);

	while(!header.empty())
	{
		colonPos = header.find(":");
		if(colonPos == string::npos || std::isspace(header[colonPos - 1]) || std:: isspace(header[0]))
		{
			cout << RED << "foud space or doesn't find :";
			break;
		}
		lineEnd = header.find(CRLF);
		if(lineEnd == string::npos)
			lineEnd = header.length();
		key = header.substr(0, colonPos);
		key.erase(0, key.find_first_not_of(" "));

		value = header.substr(colonPos + 1, lineEnd - colonPos - 1);
		value.erase(0, value.find_first_not_of(" "));
		
		std::transform(key.begin(), key.end(), key.begin(), ::tolower); // check this 
		if (key == "transfer-encoding") // check other
			std::transform(value.begin(), value.end(), value.begin(), ::tolower); // check this 
		
		HeaderData.bigMap.insert(std::make_pair(key, value));		
		header.erase(0, lineEnd + 2);
	}
	// print(HeaderData.bigMap);
	fillData(HeaderData.bigMap);
}


const string Request::getExtention(std::map<string, string> mp)
{
	std::map<string, string>::iterator it = mp.find("content-type");
	if (it != mp.end())
	{
		string str = it->second.substr(it->second.find("/") + 1, it->second.length());
		if(str == "octet-stream")
			return "py";
		return str;
	}
	else
		cout << RED << "Bad request\n" ;
	return ""; // check this 
	// GET NAME FROM TIME
}


void Request::fillData(const std::map<string, string> &mp)
{
	map<string, string>::const_iterator	lengthPos = mp.find("content-length");
	map<string, string>::const_iterator	multiPart = mp.find("content-type");
	map<string, string>::const_iterator	chunked = mp.find("transfer-encoding");
	
	RequestData.requestStat = (1);
	
	BodyData.bodyType = NONE;
	
	if (lengthPos != mp.end()){
		BodyData.bodySize = (std::atol(lengthPos->second.c_str()));
		if (BodyData.bodySize > 0)	BodyData.bodyType = BODY_SIZE;
	}
	if (mp.find("host") == mp.end())
		cout << RED << "no Host found !!\n" << RESET;

	if (multiPart != mp.end())
		HeaderData.extention = (getExtention(mp));

	bool bol = multiPart != mp.end() && multiPart->second.find("multipart/form-data;") != std::string::npos;
	if (bol)
	{
		string sep = multiPart->second.substr(multiPart->second.rfind("boundary=") + 9 , multiPart->second.length());
		BodyData.boundry = ("--" + sep + "\r\n");
		BodyData.endBoundry = ("\r\n--" + sep + "--\r\n");
		BodyData.bodyType = BOUNDARY;
	}
	
	if (chunked != mp.end())
	{
		if (chunked->second != "chunked")
			cout << RED << "not implemented\n" << RESET ;
		else if (chunked->second == "chunked" && !bol)
			BodyData.bodyType = CHUNKED;
		else if (chunked->second == "chunked" && bol)
			BodyData.bodyType = CHUNKED_BOUNDARY;
	}
}