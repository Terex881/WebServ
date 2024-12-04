#include "Request.hpp"

void parseFirstLine(string line, map<string, string> &headerMap)
{
	string word;
	string key;
	int i = 0;
	if (std::count(line.begin(),line.end(), ' ') != 2)
		cout << "erro: found 2 spaces in the first line\n";
	while(line.length() > 0)
	{
		word = line.substr(0, line.find(" ")); line.erase(0, word.length());
		key = (i == 0) ? "method" : (i == 1) ? "path" : "version";
		headerMap[key] = word; i++;
		line.erase(0, 1);
	}
	map<string, string>::iterator it = headerMap.find("method");
	if (it->second != "POST" && it->second != "GET" && it->second != "DELETE")
		cout << "501 Not IheaderMaplemented\n";
	it = headerMap.find("path");// check other characters
	if (it->second.find_first_not_of("%!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
		cout << "bad URL\n";
	it = headerMap.find("version");
	if (it->second != "HTTP/1.1\r")
		cout << "invalid version\n";
}

void trim(string &str){	
	std::string::size_type first = str.find_first_not_of("\r\n");
	std::string::size_type last = str.find_last_not_of("\r\n");
	if (first == std::string::npos || last == std::string::npos)
		return;
	str = str.substr(first, last - first + 1);
}

void parseChunkedBody(string line, std::ofstream &tmp)
{
	char *end; string res;
	// read until 0 and parse 
    while (1337)
	{
		unsigned long l = strtol(line.c_str(), &end, 16);
		line = end;
		if (l == 0)
		{
			if (!line.empty())
				cout << "error: strtol doesn't found an hexadecimal return 0"; // if the length not valid strtol fails return 0
			break;
		}
		trim(line);
		res = line.substr(0, l);
		if ((unsigned long)l != res.length())
			cout << "error: the length of the chunked data dosn't match\n";
		tmp << res;
		line.erase(0, l);
		if (line[0] != '\r' && line[1] != '\n')
			cout << "error: found 'r n' inside line\n"; // ckech if it fails to close the \nfile
		trim(line);
    }
}

void parseBoundryBody(string line, map<string, string> headerMap)
{
	string title, boundry, fileName, subBody;
	unsigned long filePos; unsigned long namePos; int i = 0;
	map<string, string>::iterator it = headerMap.find("Content-Type");
	string seperator = "--" + it->second.substr(it->second.find("=") + 1, it->second.find("\r") - it->second.find("=") - 1);
	
	while(!line.empty())
	{
		line = line.c_str() + seperator.length() + 2; // +2 to skip \r\n 
		subBody = line.substr(0, line.find(seperator));
		size_t subBodyLength = subBody.length();
		filePos = subBody.find("filename=\"");
		if (filePos != std::string::npos)
		{
			subBody.erase(0, filePos + 10);
			fileName = subBody.substr(0, subBody.find("\"\r"));
			ofstream binaryDataFile(fileName);
			trim(subBody);
			string binaryData = subBody.substr(subBody.find("\r\n\r\n")+4, subBody.length()); // add 4 to skip \r\n\r\n
			binaryDataFile << binaryData;
		}
		else if (!subBody.empty())
		{
			trim(subBody);
			ofstream file("file" + to_string(i)); i++;
			file << subBody;
		}
		line.erase(0, subBodyLength); trim(line);
	}
}

void parseBodyTypes(string line, map<string, string> headerMap)
{
	char *end;
	std::map<string, string>::iterator it;
	ofstream tmp("file_to_read.txt");
	if (headerMap.find("Transfer-Encoding")->second == "chunked\r")
		parseChunkedBody(line, tmp);
	else if (headerMap.find("Content-Length") != headerMap.end())
	{
		it = headerMap.find("Content-Length");
		if (line.length() != (unsigned long)std::strtol(line.c_str(), &end, 10))
			cout << "content-length doesn't match with size of the body\n";
		tmp << line;
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
				cout << "error: found spaces\n";
			headerMap[key] = val;
		}
		else
			cout << "error: no : founded\n";

	}
	if (headerMap.find("method")->second == "POST")
	{
		string str = ss.str();
		line.assign(str);
		// getline(ss, line, '\0'); // read body
		parseBodyTypes(line, headerMap);
	}
}