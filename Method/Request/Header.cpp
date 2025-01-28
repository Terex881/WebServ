/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:45 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/26 17:27:01 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../../Config/File_Parsing.hpp"

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
			value = key + "=" + queryStrings.substr(equalPos + 1, endPos - equalPos - 1);
			HeaderData.queryStringVec.push_back(value);
		}
		if (andPos != string::npos)	queryStrings.erase(0, endPos + 1);
		else	queryStrings.clear();
	}
	print(HeaderData.queryStringVec);
}
 
void Request::parseUri(string &str)
{
	if (str.find_first_not_of("%!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
	{
		clean(400, "bad URL");
		// RequestData.codeStatus = 400;	RequestData.requestStat = 2;
		// throw runtime_error("bad URL");
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
	size_t QMPos = str.find("?");
	if (QMPos != string::npos)
		storeQueryString(str, QMPos);
	HeaderData.url = str;
}

void Request::parseFirstLine(const string &line)
{
	string method, uri, httpVersion;
	map<string, string>::iterator it;

	if (isspace(line[0]))
	{
		clean(400, "foud space in the begening");
		// RequestData.codeStatus = 400;	RequestData.requestStat = 2;	throw runtime_error("foud space in the begening");
	}

	std::istringstream ss(line);
	ss >> method >> uri >> httpVersion;
	
	HeaderData.bigMap.insert(std::make_pair("method", method));
	HeaderData.bigMap.insert(std::make_pair("uri", uri));
	HeaderData.bigMap.insert(std::make_pair("httpVersion", httpVersion));

	HeaderData.requestMethod = method;
	if (method != "POST" && method != "GET" && method != "DELETE")
	{
		clean(501, "501 Not Implementedddd");
		// RequestData.codeStatus = 501;	RequestData.requestStat = 2;	throw runtime_error("501 Not Implementedddd");
	}
	if ((it = HeaderData.bigMap.find("uri")) != HeaderData.bigMap.end())
		parseUri(it->second);
	if ((it = HeaderData.bigMap.find("httpVersion")) != HeaderData.bigMap.end() && it->second != "HTTP/1.1")
	{
		clean(505, "error: invalid version");
		// RequestData.codeStatus = 505;	RequestData.requestStat = 2;	throw runtime_error("error: invalid version");
	}
}

void Request::fillHeaderMap(string &header)
{
	size_t colonPos;

	string key, line, value;

	while(!header.empty())
	{
		line = header.substr(0, header.find(CRLF));
		colonPos = line.find(":");
		
		key = line.substr(0, colonPos);
		if(key.find_first_of(" \t\r\f\v\n") != NP || colonPos == NP || isspace(line[colonPos - 1]) || isspace(line[0])) {
			clean(400, "foud space or doesn't find :");
			// RequestData.codeStatus = 400;	RequestData.requestStat = 2;
			// throw runtime_error("foud space or doesn't find :");
		}

		size_t valEndPos  = line.find_first_not_of(' ', colonPos + 1);
		if (valEndPos == NP)	valEndPos = 0;
		value = line.substr(valEndPos, line.find_last_not_of(' ') - valEndPos + 1);
	
		std::transform(key.begin(), key.end(), key.begin(), ::tolower); // check this 
		if (key == "transfer-encoding") // check other
			std::transform(value.begin(), value.end(), value.begin(), ::tolower); // check this 

		fillData(key, value);
		HeaderData.bigMap.insert(std::make_pair(key, value));	
		header.erase(0, line.length() + 2);
	}
}

string	find_extension(string url)
{
	size_t at = url.find(".");
	string extension;

	if (at != NP)
	{
		// cout <<GREEN << HeaderData.url << RESET<< endl;
		string current = url.substr(at);
		// cout <<"... " << current << endl;
		extension = current.substr(0, (current.find("/") != NP ? current.find("/") : current.length()));
		cout <<YELLOW << extension << RESET<< endl;
	}
	return extension;
}


void Request::achref()
{
	DynamicStruct	location;
	DynamicStruct	server;
	// std::vector<string> redirection;

	
	configFileObj.getLocationByPortAndUrl(HeaderData.port, configFileObj.correct_url(HeaderData.url), location, server);
	if (!location.values.size() || !server.values.size())
	{
		if (configFileObj.correct_url(HeaderData.url)[0] == '/')
		{
			string sub_route = configFileObj.correct_url(HeaderData.url).substr(1);
			// cout <<GREEN << "sub_route : " << sub_route << RESET<< endl;
			configFileObj.getLocationByPortAndUrl(HeaderData.port, configFileObj.correct_url("/"), location, server);
			// HeaderData.url = 
		}
	}
	if (location.values.size())
	{
		cout << GREEN << "Checking_1 ...."<< RESET << endl;

		// get max bosy size 
		location_data l_data = configFileObj.get_location_val(location);
		RequestData.maxBodySize = atol(server.values["client_max_body_size"].c_str()) * 10000000;
		RequestData.fileLocation = l_data.root;
		// redirection
		if (!l_data.rturn.empty())
			RequestData.redirection = configFileObj.split_1(l_data.rturn);
		HeaderData.url = configFileObj.correct_url(HeaderData.url);
	
		string	final_url = configFileObj.correct_url(l_data.root + "/" + HeaderData.url);
		HeaderData.url = final_url;

		string extension = find_extension(HeaderData.url);
		if ((!location.values[".py"].empty() && extension == ".py") || (!location.values[".php"].empty() && extension == ".php"))
		{
			RequestData.isCgi = true;
		}
		cout << RED << "1 "<< HeaderData.url << RESET << endl;
		if (RequestData.isCgi)
		{
			cout << location.values["path"] << " cgi : " << RequestData.isCgi << endl;
			// cout << "ext : " <<  HeaderData.extension << endl;
			size_t at = HeaderData.url.find(".");
			if (at != NP)
			{
				// cout <<GREEN << HeaderData.url << RESET<< endl;
				string current = HeaderData.url.substr(at);
				// cout <<"... " << current << endl;
				HeaderData.extension = current.substr(0, (current.find("/") != NP ? current.find("/") : current.length()));
				// cout << " -extension- " << HeaderData.extension << endl;
				RequestData.executable_file = location.values[HeaderData.extension].substr(0, location.values[HeaderData.extension].find(';'));
				if (RequestData.executable_file.empty() || (HeaderData.extension != ".py" && HeaderData.extension != ".php"))
				{
					RequestData.isCgi = false;
					return;
				}
				// cout << GREEN << " :: "<< RequestData.executable_file << RESET << endl;
				RequestData.pathInfo = current.substr(current.find("/") != NP ? current.find("/") : HeaderData.extension.length());
				// cout << "pathInfo : " << RequestData.pathInfo << endl;
				HeaderData.url = HeaderData.url.substr(0, at + HeaderData.extension.length());
				// cout << YELLOW << HeaderData.url << RESET<< endl;
			}
			else
			{
				RequestData.isCgi = false;
				// default_page
				if (!server.values["index"].empty())
				{
					RequestData.default_page = location.values["root"].substr(0, location.values["root"].find(";")) +"/" + server.values["index"];
					cout << GREEN << "root : " << location.values["root"] << endl;
					cout << GREEN << "Default : "<< RequestData.default_page << endl;
				}
				cout << GREEN << "Index Oder autoIndexxx" << RESET << endl;
			}
		}
		else
		{
			// default_page
			if (!server.values["index"].empty())
			{
				RequestData.default_page = location.values["root"].substr(0, location.values["root"].find(";")) +"/" + server.values["index"];
				cout << GREEN << "root : " << location.values["root"] << endl;
				cout << GREEN << "Default : "<< RequestData.default_page << endl;
			}
			cout << GREEN << "Index Oder autoIndex" << RESET << endl;
		}

		
			cout << GREEN << "Checking ...."<< RESET << endl;
		if (l_data.methods.size() && find(l_data.methods.begin(), l_data.methods.end(), HeaderData.requestMethod) == l_data.methods.end())
		{
			cout << GREEN << "NOT ALLOWED"<< RESET << endl;
			clean(405, "Method Not Allowed");
			// RequestData.codeStatus = 405;	RequestData.requestStat = 2;	throw runtime_error("Method Not Allowed");
		}
		if (!l_data.directory_listing.empty() && l_data.directory_listing == "on;")
			RequestData.isDirListening = true;
		if (!l_data.rturn.empty())
			RequestData.isRedirect = true;
		if (!server.values["server_name"].empty())
			RequestData.serverName = server.values["server_name"];	
		
	}
	else
	{
		// RequestData.codeStatus = 404;	RequestData.requestStat = 2;	throw runtime_error("location not found");
		clean(404, "location not found");
	}
}

void Request::parseHeader(string &header)
{
	size_t			pos = header.find(CRLF);
	string			firstLine = header.substr(0, pos);

	

	// cout << YELLOW << firstLine << RESET << endl;
	parseFirstLine(firstLine);
	header.erase(0, pos + 2);	
	BodyData.bodyType = NONE;
	fillHeaderMap(header);
	// cout << RED << "HEREE:" << HeaderData.url << RESET << endl;

	achref();
	// print(HeaderData.bigMap);
	getTypes(HeaderData.bigMap);
	
}

void Request::fillData(const string &key, const string &value)
{
	size_t Pos = 0;
	// if  (key == "content-type" && (Pos = value.find("/")) && Pos != NP)
	// 	HeaderData.extension = value.substr(Pos + 1, value.length());
	if (key == "content-length")
		BodyData.bodySize = (std::atol(value.c_str()));
	if (key == "host" && (Pos = value.find(":")) != NP)
		HeaderData.port = value.substr(Pos + 1, 10);
	if (key == "cookie")
		HeaderData.queryStringVec.push_back("Cookie=" + value);
}

void Request::getTypes(const std::map<string, string> &mp)
{
	map<string, string>::const_iterator	multiPart = mp.find("content-type");
	map<string, string>::const_iterator	chunked = mp.find("transfer-encoding");
	map<string, string>::const_iterator	alive = mp.find("connection");
	
	if (alive != mp.end() && alive->second == "close")
		HeaderData.isAlive = false;
	else if (alive != mp.end())
		HeaderData.isAlive = true;
		
	RequestData.requestStat = 1;

	if (BodyData.bodySize && RequestData.isCgi)
		BodyData.bodyType = BODY_SIZE;
	
	// cout << RED << RequestData.maxBodySize << RESET << endl;
	// cout << RED << BodyData.bodySize << RESET << endl;

	

	if (BodyData.bodySize > RequestData.maxBodySize)
	{
		RequestData.isCgi = false;
		clean(413, "Content Too Large");
		// RequestData.codeStatus = 400;
		// RequestData.requestStat = 2;
		// throw runtime_error("MAX bady szie");
	}

	if (HeaderData.port.empty())
		clean(404, "no Host found !!");
		// {RequestData.codeStatus = 404;	RequestData.requestStat = 2;	throw runtime_error("no Host found !!");} // chek

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
		if (chunked->second == "chunked" && !bol && RequestData.isCgi)
			BodyData.bodyType = CHUNKED;
		else if (chunked->second == "chunked" && bol)
			BodyData.bodyType = CHUNKED_BOUNDARY;
		else
			clean(501, "Bad Request2");
			// {RequestData.codeStatus = 501;	RequestData.requestStat = 2;	throw runtime_error("Bad Request2");}
	}
	else if (BodyData.bodySize && !RequestData.isCgi && !bol)
			clean(501, "Bad Request1");
		// {RequestData.codeStatus = 501;	RequestData.requestStat = 2;	throw runtime_error("Bad Request1");}

	cout << RED << "HERE:" << HeaderData.url << RESET << endl;
}