/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:45 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/03 13:18:07 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Header.hpp"
#include "Request.hpp"

void	Header::setAttay(Request *reqPtr)
{
	ataty = reqPtr;
}

void Header::parseUri(string &str)
{
	if (str.find_first_not_of("%!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
		cout << RED << "bad URL" << std::endl;
	
	size_t QMPos = str.find("?");
	if (QMPos != string::npos)
	{
		queryStrings = str.substr(QMPos, str.length());
		str.erase(QMPos);
		cout << YELLOW << str <<	RESET << endl;
		cout << RED << queryStrings << RESET << endl;
	}

}

void Header::parseFirstLine(string &line)
{
	string method, uri, httpVersion;
	map<string, string>::iterator it;

	if (isspace(line[0]))
		cout << RED << "foud space in the begening\n";

	std::istringstream ss(line);
	ss >> method >> uri >> httpVersion;
	
	bigMap.insert(std::make_pair("method", method));
	bigMap.insert(std::make_pair("uri", uri));
	bigMap.insert(std::make_pair("httpVersion", httpVersion));
	
	if ((it = bigMap.find("method")) != bigMap.end() && it->second != "POST" && it->second != "GET" && it->second != "DELETE")
		cout << RED << "501 Not Implemented" << std::endl;
	if ((it = bigMap.find("uri")) != bigMap.end())
		parseUri(it->second);
	if ((it = bigMap.find("httpVersion")) != bigMap.end() && it->second != "HTTP/1.1")
		cout << RED << "error: invalid version" << std::endl;
}

void Header::parseHeader(string &header)
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
		

		bigMap.insert(std::make_pair(key, value));		
		header.erase(0, lineEnd + 2);
	}
	// print(bigMap);
	fillData(bigMap);
}


const string Header::getExtention(std::map<string, string> mp)
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


void Header::fillData(const std::map<string, string> &mp)
{
	map<string, string>::const_iterator	lengthPos = mp.find("content-length");
	map<string, string>::const_iterator	multiPart = mp.find("content-type");
	map<string, string>::const_iterator	chunked = mp.find("transfer-encoding");
	ataty->setStat(1);
	ataty->setType(3);
	
	if (lengthPos != mp.end())
		ataty->setSize(std::atol(lengthPos->second.c_str()));
	if (mp.find("host") == mp.end())
		cout << RED << "no Host found !!\n" << RESET;

	if (multiPart != mp.end())
		ataty->setEx(getExtention(mp));
	else
		cout << RED << "no type founded" << endl;

	bool bol = multiPart != mp.end() && multiPart->second.find("multipart/form-data;") != std::string::npos;
	if (bol)
	{
		string sep = multiPart->second.substr(multiPart->second.rfind("boundary=") + 9 , multiPart->second.length());
		ataty->setB("--" + sep + "\r\n");
		ataty->setEndB("\r\n--" + sep + "--\r\n");
		ataty->setType(0);
	}
	if (chunked != mp.end())
	{
		if (chunked->second != "chunked")
			cout << RED << "not implemented\n" << RESET ;
		else if (chunked->second == "chunked" && !bol)
			ataty->setType(1);
		else if (chunked->second == "chunked" && bol)
		{
			ataty->setType(2);
		}
	}
	// cout << YELLOW << "Before:" << ataty->getType() << endl;
}