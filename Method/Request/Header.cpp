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
}
 
void Request::parseUri(string &str)
{
	if (str.find_first_not_of("%!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos && RequestData.errorFlag)
		clean(400, "Bad Request");
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
	HeaderData.urlFinal = str;
	HeaderData.url = str;
}

void Request::parseFirstLine(const string &line)
{
	string method, uri, httpVersion;
	if (isspace(line[0]) && RequestData.errorFlag)
		clean(400, "Bad Request");

	std::istringstream ss(line);
	ss >> method >> uri >> httpVersion;	
	if ((method.empty() || uri.empty() || httpVersion.empty()) && RequestData.errorFlag)
		clean(400, "Bad Request");
	HeaderData.bigMap.insert(std::make_pair("REQUEST_METHOD", method));
	HeaderData.bigMap.insert(std::make_pair("URI", uri));
	HeaderData.bigMap.insert(std::make_pair("HTTP_VERSION", httpVersion));

	HeaderData.requestMethod = method;
	if (method != "GET" && method != "HEAD" && method != "OPTIONS" && method != "TRACE" && method != "PUT" && method != "DELETE" && method != "POST" && method != "PATCH" && method != "CONNECT" && RequestData.errorFlag)
		clean(400, "Bad Request");
	else if (method != "POST" && method != "GET" && method != "DELETE" && RequestData.errorFlag)
		clean(501, "Not Implemented");
	parseUri(uri);
	if (httpVersion != "HTTP/0.9" && httpVersion != "HTTP/1.0" && httpVersion != "HTTP/1.1" && httpVersion != "HTTP/2" && httpVersion != "HTTP/3" && RequestData.errorFlag)
		clean(400, "Bad Request");
	else if (httpVersion != "HTTP/1.1" && RequestData.errorFlag)
		clean(505, "HTTP Version Not Supported");
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
		if((key.find_first_of(" \t\r\f\v\n") != NP || colonPos == NP || isspace(line[colonPos - 1]) || isspace(line[0])) && RequestData.errorFlag)
			clean(400, "Bad Request");

		size_t valEndPos  = line.find_first_not_of(' ', colonPos + 1);
		if (valEndPos == NP)	valEndPos = 0;
		value = line.substr(valEndPos, line.find_last_not_of(' ') - valEndPos + 1);

	
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
		if (key == "TRANSFER_ENCODING" || key == "CONNECTION")
			std::transform(value.begin(), value.end(), value.begin(), ::tolower);
		std::replace(key.begin(), key.end(), '-', '_');

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
		string current = url.substr(at);
		extension = current.substr(0, (current.find("/") != NP ? current.find("/") : current.length()));
	}
	return extension;
}

void Request::parseConfigInHeader()
{
	DynamicStruct	location;
	DynamicStruct	server;

	string server_nama = RequestData.hostName;
	configFileObj.getLocationByPortAndUrl(HeaderData.port, configFileObj.correct_url(HeaderData.url), location, server, server_nama);

	if (!location.values.size() || !server.values.size())
	{
		if (configFileObj.correct_url(HeaderData.url)[0] == '/')
			configFileObj.getLocationByPortAndUrl(HeaderData.port, configFileObj.correct_url("/"), location, server, server_nama);
	}
	if (location.values.size())
	{
		if (!location.values["upload_path"].empty())
			location.values["upload_path"] = location.values["upload_path"].substr(0, location.values["upload_path"].find(";"));
		else
		 	location.values["upload_path"] = "/Users/sdemnati/goinfre/UP";
		configFileObj.server = server;
		
		location_data l_data = configFileObj.get_location_val(location);
		RequestData.maxBodySize = atol(server.values["client_max_body_size"].c_str()) * 10000000;
		RequestData.fileLocation = location.values["upload_path"];

		if (!l_data.rturn.empty())
			RequestData.redirection = configFileObj.split_1(l_data.rturn);
		HeaderData.url = configFileObj.correct_url(HeaderData.url);
	
		HeaderData.url = configFileObj.correct_url(l_data.root + "/" + HeaderData.url);

		string extension = find_extension(HeaderData.url);
		if ((!location.values[".py"].empty() && extension == ".py") || (!location.values[".php"].empty() && extension == ".php"))
		{
			RequestData.isCgi = true;
		}
		if (RequestData.isCgi)
		{
			size_t at = HeaderData.url.find(".");
			if (at != NP)
			{
				string current = HeaderData.url.substr(at);
				HeaderData.extension = current.substr(0, (current.find("/") != NP ? current.find("/") : current.length()));
				RequestData.executable_file = location.values[HeaderData.extension].substr(0, location.values[HeaderData.extension].find(';'));
				if (RequestData.executable_file.empty() || (HeaderData.extension != ".py" && HeaderData.extension != ".php"))
				{
					RequestData.isCgi = false;
					return;
				}
				RequestData.pathInfo = current.substr(current.find("/") != NP ? current.find("/") : HeaderData.extension.length());
				HeaderData.url = HeaderData.url.substr(0, at + HeaderData.extension.length());
			}
			else
			{
				RequestData.isCgi = false;
				if (!server.values["index"].empty())
					RequestData.default_page = location.values["root"].substr(0, location.values["root"].find(";")) +"/" + server.values["index"];
			}
		}
		else {
			if (!server.values["index"].empty())
				RequestData.default_page = location.values["root"].substr(0, location.values["root"].find(";")) +"/" + server.values["index"];
		}
		if (l_data.methods.size() && find(l_data.methods.begin(), l_data.methods.end(), HeaderData.requestMethod) == l_data.methods.end() && RequestData.codeStatus == 200 && RequestData.errorFlag)
			clean(405, "Method Not Allowed");
		if (!l_data.directory_listing.empty() && l_data.directory_listing == "on;")
			RequestData.isDirListening = true;
		if (!l_data.rturn.empty())
			RequestData.isRedirect = true;
		if (!server.values["server_name"].empty())
			RequestData.serverName = server.values["server_name"];	
	}
	else if (RequestData.errorFlag)
		clean(404, "Not Found");
	

}

void Request::parseHeader(string &header)
{
	size_t	pos = header.find(CRLF);
	string	firstLine = header.substr(0, pos);
	
	parseFirstLine(firstLine);
	header.erase(0, pos + 2);	
	
	fillHeaderMap(header);
	parseConfigInHeader();
	getTypes(HeaderData.bigMap);
}

void Request::fillData(string &key, const string &value)
{
	size_t Pos = 0;
	if (key == "CONTENT_LENGTH")
		BodyData.bodySize = (std::atol(value.c_str()));
	if (key == "HOST" && (Pos = value.find(":")) != NP)
	{
		RequestData.hostName = value.substr(0, Pos);
		HeaderData.port = value.substr(Pos + 1, 10);
		key = "SERVER_NAME";
	}
	if (key == "COOKIE")
		HeaderData.queryStringVec.push_back("Cookie=" + value);	
}

void Request::getTypes(const std::map<string, string> &mp)
{
	map<string, string>::const_iterator	multiPart = mp.find("CONTENT_TYPE");
	map<string, string>::const_iterator	chunked = mp.find("TRANSFER_ENCODING");
	map<string, string>::const_iterator	alive = mp.find("CONNECTION");
	
	if (alive != mp.end() && alive->second == "close")
		HeaderData.isAlive = false;
	else if (alive != mp.end())
		HeaderData.isAlive = true;
		
	RequestData.requestStat = 1;

	if (BodyData.bodySize && RequestData.isCgi)
		BodyData.bodyType = BODY_SIZE;
	
	if (BodyData.bodySize > RequestData.maxBodySize && RequestData.errorFlag)
	{
		RequestData.isCgi = false;
		clean(413, "Content Too Large");
	}

	if (HeaderData.port.empty() && RequestData.errorFlag)
		clean(400, "Bad Request");

	bool bol = multiPart != mp.end() && multiPart->second.find("multipart/form-data;") != std::string::npos;
	if (bol && HeaderData.requestMethod == "POST")
	{
		string sep = multiPart->second.substr(multiPart->second.rfind("boundary=") + 9 , multiPart->second.length());
		BodyData.boundry = ("--" + sep + "\r\n");
		BodyData.endBoundry = ("\r\n--" + sep + "--\r\n");
		BodyData.bodyType = BOUNDARY;
	}
	if (chunked != mp.end() && HeaderData.requestMethod == "POST")
	{
		if (chunked->second == "chunked" && !bol && RequestData.isCgi)
			BodyData.bodyType = CHUNKED;
		else if (chunked->second == "chunked" && bol)
			BodyData.bodyType = CHUNKED_BOUNDARY;
		else if (RequestData.errorFlag)
			clean(501, "Not Implemented");
	}
	else if (BodyData.bodySize && !RequestData.isCgi && !bol && HeaderData.requestMethod == "POST" && RequestData.errorFlag)
		clean(501, "Not Implemented");
	
	if (RequestData.codeStatus != 200 && RequestData.codeStatus != 201)
	{
		RequestData.requestStat = 2;
		throw std::runtime_error(RequestData.codeStatusMap[RequestData.codeStatus]);
	}
}