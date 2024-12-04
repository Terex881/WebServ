#include "Request.hpp"
#include <algorithm>
#include <cctype>

void parseFirstLine(string line, map<string, string> &headerMap)
{
	string word;
	string key;
	int i = 0;
	if (std::count(line.begin(),line.end(), ' ') != 2)
		throw invalid_argument("2 spaces1");
	while(line.length() > 0)
	{
		word = line.substr(0, line.find(" ")); line.erase(0, word.length());
		key = (i == 0) ? "method" : (i == 1) ? "path" : "version";
		headerMap[key] = word; i++;
		line.erase(0, 1);
	}
	map<string, string>::iterator it = headerMap.find("method");
	if (it->second != "POST" && it->second != "GET" && it->second != "DELETE")
		throw logic_error("501 Not IheaderMaplemented");
	it = headerMap.find("path");
	if (it->second.find_first_not_of("!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
		throw logic_error("bad URL");
	it = headerMap.find("version");
	if (it->second != "HTTP/1.1\r")
		throw invalid_argument("invalid version");

}
	// for(map<string, string>::iterator it = headerMap.begin(); it != headerMap.end(); it++)
	// 	cout << it->first<< "    " << it->second << endl;


void trim(string &str)
{	
	std::string::size_type first = str.find_first_not_of("\r\n");
	std::string::size_type last = str.find_last_not_of("\r\n");
	if (first == std::string::npos || last == std::string::npos)
		return;
	str = str.substr(first, last - first + 1);
}

void parseChunkedBody(string line, std::ofstream &theaderMap)
{
	char *end; string res;
    while (1337)
	{
		long l = strtol(line.c_str(), &end, 16);
		line = end;
		if (l == 0){
			if (!line.empty())
				throw invalid_argument("error3"); // if the length not valid strtol fails return 0
			break;
		}
		trim(line);
		res = line.substr(0, l);
		if ((unsigned long)l != res.length())
			throw invalid_argument("error1");
		theaderMap << res;
		line.erase(0, l);
		if (line[0] != '\r' && line[1] != '\n')
			throw invalid_argument("error2"); // ckech if it fails to close the file
		trim(line);
    }
}


void parseBoundryBody(string line, map<string, string> headerMap)
{
	map<string, string>::iterator it = headerMap.find("Content-Type");
	string title, boundry, delimter, data, subline, name, fileName, subBody;
	unsigned long filePos; unsigned long namePos;
	it = headerMap.find("Content-Type");
	string seperator = "--" + it->second.substr(it->second.find("=") + 1, it->second.find("\r") - it->second.find("=") - 1);
	line = line.c_str() + seperator.length();
	subBody = line.substr(0, line.find(seperator));
	// cout << subBody << endl;
	filePos = subBody.find("filename=\"");
	if (filePos != std::string::npos)
	{
		subBody.erase(0, filePos + 10);
		fileName = subBody.substr(filePos, subBody.find("\"\r"));
		cout << fileName << endl;
	}


	

}

void parseBodyTypes(string line, map<string, string> headerMap)
{
	char *end;
	std::map<string, string>::iterator it;
	ofstream theaderMap("file_to_read.txt");
	if (headerMap.find("Transfer-Encoding")->second == "chunked\r")
		parseChunkedBody(line, theaderMap);
	else if (headerMap.find("Content-Length") != headerMap.end())
	{
		it = headerMap.find("Content-Length");
		if (line.length() != (unsigned long)std::strtol(line.c_str(), &end, 10))
			throw invalid_argument("erro");
		theaderMap << line;
    }
	else if (headerMap.find("Content-Type") != headerMap.end())
		parseBoundryBody(line, headerMap);
}

Request::Request(string str)
{
	string line, key, val;
	stringstream ss(str);
	getline(ss, line, '\n');
	parseFirstLine(line, headerMap); // store first line in the map
	while(getline(ss, line, '\n') && line != "\r")
	{
		if (line.find(": ") != std::string::npos)
		{
			key = line.substr(0, line.find(":"));
			val = line.substr(line.find(":") + 2, line.find("\r"));
			if (isspace(key[0]) || isspace(key[key.length()-1]) || isspace(val[0]) || isspace(val[val.length()-2]))
				throw invalid_argument("bad request123");
			headerMap[key] = val;
		}
		else
			throw invalid_argument("bad request");

	}
	if (headerMap.find("method")->second == "POST")
	{
		getline(ss, line, '\0'); // read body
		parseBodyTypes(line, headerMap);
	}
}