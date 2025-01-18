/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:45 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/15 16:04:20 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../../Config/File_Parsing.hpp"
#include <algorithm>

void Request::storeQueryString(string &str, const size_t &QMPos)
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
	{
		RequestData.codeStatus = 400; // check default path
		RequestData.requestStat = 2;
		cout << RED << "bad URL" << std::endl;
		return;
	}

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
	if (str.find("cgi-bin/") != string::npos)
		RequestData.isCgi = true;
}

void Request::parseFirstLine(const string &line)
{
	string method, uri, httpVersion;
	map<string, string>::iterator it;

	if (isspace(line[0]))
	{
		RequestData.codeStatus = 400; // burp
		RequestData.requestStat = 2;
		cout << RED << "foud space in the begening\n";
		return;
	}

	std::istringstream ss(line);
	ss >> method >> uri >> httpVersion;
	
	HeaderData.bigMap.insert(std::make_pair("method", method));
	HeaderData.bigMap.insert(std::make_pair("uri", uri));
	HeaderData.bigMap.insert(std::make_pair("httpVersion", httpVersion));


	HeaderData.requestMethod = method;
	if (HeaderData.requestMethod != "POST" && HeaderData.requestMethod != "GET" && HeaderData.requestMethod != "DELETE")
	{
		RequestData.codeStatus = 501;
		RequestData.requestStat = 2;
		cout << RED << "501 Not Implemented" << std::endl;
		return;
	}
	if ((it = HeaderData.bigMap.find("uri")) != HeaderData.bigMap.end())
		parseUri(it->second);
	if ((it = HeaderData.bigMap.find("httpVersion")) != HeaderData.bigMap.end() && it->second != "HTTP/1.1")
	{
		RequestData.codeStatus = 505;
		RequestData.requestStat = 2;
		cout << RED << "error: invalid version" << std::endl;
	}
}

void Request::fillHeaderMap(string &header)
{
	size_t colonPos, lineEnd;

	string key, line, value;

	while(!header.empty())
	{
		line = header.substr(0, header.find(CRLF));
		colonPos = line.find(":");
		
		if(colonPos == string::npos || std::isspace(line[colonPos - 1]) || std:: isspace(line[0])) {
			RequestData.codeStatus = 400; // check in burop
			RequestData.requestStat = 2;
			cout << RED << "foud space or doesn't find :";
			break;
		}
		// check mli7
		key = line.substr(0, colonPos);
		size_t valEndPos  = line.find_first_not_of(' ', colonPos + 1);
		if (valEndPos == string::npos)	valEndPos = 0;
		value = line.substr(valEndPos, line.find_last_not_of(' ') - valEndPos + 1);
	
		std::transform(key.begin(), key.end(), key.begin(), ::tolower); // check this 
		if (key == "transfer-encoding") // check other
			std::transform(value.begin(), value.end(), value.begin(), ::tolower); // check this 

		fillData(key, value);		
		HeaderData.bigMap.insert(std::make_pair(key, value));	
		header.erase(0, line.length() + 2);
	}
}

void Request::parseHeader(string &header)
{
	size_t			pos = header.find(CRLF);
	string			firstLine = header.substr(0, pos);
	DynamicStruct	location;
	DynamicStruct	server;
	
	parseFirstLine(firstLine);
	header.erase(0, pos + 2);	
	BodyData.bodyType = NONE;
	fillHeaderMap(header);

	configFileObj.getLocationByPortAndUrl(HeaderData.port, configFileObj.correct_url(HeaderData.url), location, server);
	if (!location.values.size() || !server.values.size())
	{
		RequestData.codeStatus = 404;
		RequestData.requestStat = 2;
		cout << RED << "location not found" << RESET << endl;
	}
	else
	{
		location_data l_data = configFileObj.get_location_val(location);
		HeaderData.url = configFileObj.correct_url(HeaderData.url);
	
		string	after_location = HeaderData.url.substr(l_data.path.length());
		string	after_slash = after_location.substr(after_location.find_last_of('/') + 1);
		string	final_url = configFileObj.correct_url(l_data.root + "/" + after_slash);

		HeaderData.url = final_url;

		if (l_data.methods.size() && find(l_data.methods.begin(), l_data.methods.end(), HeaderData.requestMethod) == l_data.methods.end())
		{
			RequestData.codeStatus = 501; //check this
			RequestData.requestStat = 2;
			return;
		}
		if (!l_data.directory_listing.empty() && l_data.directory_listing == "on;")
			RequestData.isDirListening = true;
		if (!l_data.rturn.empty())
			RequestData.isRedirect = true;
		if (!server.values["server_name"].empty())
			RequestData.serverName = server.values["server_name"];	
	}
	getTypes(HeaderData.bigMap);
		/**
			check methods, if empty() use the default ones, else uses only the available onces -->
		*/
		// if (!location.values["methods"].size())
		// is directory_listing is ON
		// is return is exist (location.values["return"])
		// the default val of "client_max_body_size" is 1
		// save "server_name" server.values["server_name"], if empty make a default one
	// std::cout << location.values["path"] << std::endl;
}

void Request::fillData(const string &key, const string &value)
{
	if  (key == "content-type")
	{
		HeaderData.extension = value.substr(value.find("/") + 1, value.length());
		if(HeaderData.extension == "octet-stream")
			HeaderData.extension = "py";
	}
	if (key == "content-length")
	{
		BodyData.bodySize = (std::atol(value.c_str()));
		if (BodyData.bodySize > 0)	BodyData.bodyType = BODY_SIZE;
	}
	if (key == "host")
	{
		HeaderData.port = value.substr(value.find(":") + 1, 10); // check if there no :
		
		// std::vector<dt>::iterator it = geto().host_port.begin();
		// for(; it != geto().host_port.end(); it++)
		// {
		// 	if (it->val == HeaderData.port)
		// 	{
		// 		RequestData.codeStatus = 400; // check this
		// 		RequestData.requestStat = 2;
		// 		cout << RED << "OK\n" << RESET;
		// 	}
		// }	
	}
}

void Request::getTypes(const std::map<string, string> &mp)
{
	map<string, string>::const_iterator	multiPart = mp.find("content-type");
	map<string, string>::const_iterator	chunked = mp.find("transfer-encoding");
	RequestData.requestStat = 1;
	
	if (HeaderData.port.empty())
	{
		RequestData.codeStatus = 400;
		RequestData.requestStat = 2;
		cout << RED << "no Host found !!\n" << RESET;
	}

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
		{
			RequestData.codeStatus = 501;
			RequestData.requestStat = 2;
			cout << RED << "not implemented\n" << RESET ;
		}
		else if (chunked->second == "chunked" && !bol)
			BodyData.bodyType = CHUNKED;
		else if (chunked->second == "chunked" && bol)
			BodyData.bodyType = CHUNKED_BOUNDARY;
	}
}