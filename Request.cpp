#include "Request.hpp"
#include <cstddef>
#include <fstream>
#include <ostream>
#include <sys/stat.h>

size_t Request::size1 = 0;

void parseFirstLine(string line, map<string, string> &headerMap)
{
	string word;
	string key;
	int i = 0;
	if (std::count(line.begin(),line.end(), ' ') != 2)
		cout << "\033[31merro: found 2 spaces in the first line\033[0m" << std::endl;

	while(line.length() > 0)
	{
		word = line.substr(0, line.find(" ")); line.erase(0, word.length());
		key = (i == 0) ? "method" : (i == 1) ? "path" : "version";
		headerMap[key] = word; i++;
		line.erase(0, 1);
	}
	map<string, string>::iterator it = headerMap.find("method");
	if (it->second != "POST" && it->second != "GET" && it->second != "DELETE")
		cout << "\033[31m501 Not IheaderMaplemented\033[0m" << std::endl;

	it = headerMap.find("path");// check other characters
	if (it->second.find_first_not_of("%!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
		cout << "\033[31mbad URL\033[0m" << std::endl;

	it = headerMap.find("version");
	if (it->second != "HTTP/1.1\r")
		cout << "\033[31merror: invalid version\033[0m" << std::endl;

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
				cout << "\033[31merror: strtol doesn't found an hexadecimal return 0\033[0m" << std::endl; // if the length not valid strtol fails return 0
			break;
		}
		trim(line);
		res = line.substr(0, l);
		if ((unsigned long)l != res.length())
			cout << "\033[31merror: the length of the chunked data dosn't match\033[0m" << std::endl;

		tmp << res;
		line.erase(0, l);
		if (line[0] != '\r' && line[1] != '\n')
			cout << "\033[31error: found 'r n' inside line\033[0m" << std::endl; // ckech if it fails to close the \nfile

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

void Request::parseBodyTypes(string line, map<string, string> headerMap)
{
	char *end;
	std::map<string, string>::iterator it;
	ofstream tmp("file_to_read.py");
	if (headerMap.find("Transfer-Encoding")->second == "chunked\r")
		parseChunkedBody(line, tmp);
	
	// if (headerMap.find("Content-Length") != headerMap.end())
	if (size1 != 0)
	{
		// it = headerMap.find("Content-Length");
		if (line.length() != size1)
			cout << "\033[31mcontent-length doesn't match with size of the body\033[0m" << std::endl;
		tmp << line;
    }
	// else if (headerMap.find("Content-Type") != headerMap.end())
	// 	parseBoundryBody(line, headerMap);
}

Request::Request(string request)
{
	static int i;
	string line, key, val;
	static string body;
	stringstream ss(request);
	
	if (!i)
	{
		getline(ss, line, '\n');
		parseFirstLine(line, headerMap); // store first line in the map
		while(getline(ss, line, '\n') && line != "\r")
		{
			if (line.find(": ") != std::string::npos)
			{
				key = line.substr(0, line.find(":"));
				val = line.substr(line.find(":") + 2, line.find("\r"));
				if (isspace(key[0]) || isspace(key[key.length()-1]) || isspace(val[0]) || isspace(val[val.length()-2]))
					cout << "\033[31merror: found spaces\033[0m" << std::endl;
				headerMap[key] = val;
			}
			else
				cout << "\033[31merror: no : founded\033[0m" << std::endl;
		}
		i = 1;
		size1 = stoi(headerMap.find("Content-Length")->second);
	}
	while(getline(ss, line, '\n'))
	{
		body += line;
		if (!ss.eof())
			body += "\n";	
	}

	// ofstream s("out.py");
	// s << body;

	// cout << "---->\033[31m" << body.length() << "\033[0m" << std::endl;
	
	ofstream file("salah.py");
	file << "body lenth is :" << body.length() << endl;
	file << "size is :" << size1;

	
	if (size1 == body.length())
		parseBodyTypes(body, headerMap);
}