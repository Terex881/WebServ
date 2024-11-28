#include "Request.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
// #include <vector>

void getFirstLine(string line, map<string, string> &mp)
{
	stringstream ss(line);
	string word ;
	ss >> word;
	mp["method"] = word;
	ss >> word;
	mp["path"] = word;
	ss >> word;
	mp["version"] = word;
}


void trim(string &str)
{	
	std::string::size_type first = str.find_first_not_of("\r\n");
	std::string::size_type last = str.find_last_not_of("\r\n");
	if (first == std::string::npos || last == std::string::npos)
		exit(1);
	else
		str = str.substr(first, last - first + 1);
}

void parseChunkedBody(string line, std::ofstream &tmp)
{
	char *end; string res;
    while (1337)
	{
		long l = strtol(line.c_str(), &end, 16);
		line = end;
		if (l == 0){
			if (!line.empty())
				throw invalid_argument("error3"); // if the length not valid stdtol fails return 0
			break;
		}
		trim(line);
		res = line.substr(0, l);
		if (l != res.length())
			throw invalid_argument("error1");
		tmp << res;
		line.erase(0, l);
		if (line[0] != '\r' && line[1] != '\n')
			throw invalid_argument("error2"); // ckech if it fails to close the file
		trim(line);
    }
}

Request::Request(string str)
{
	ofstream tmp("file_to_read.txt");
	string line, key, val;
	stringstream ss(str);
	getline(ss, line, '\n');
	getFirstLine(line, mp); // store first line in the map
	while(getline(ss, line, '\n') && line != "\r")
	{
		if (line.find(": ") != std::string::npos)
		{
			key = line.substr(0, line.find(":"));
			val = line.substr(line.find(":") + 2, line.find("\r"));
			mp[key] = val;
		}
		else
			throw invalid_argument("bad request");
	}
	getline(ss, line, '\0'); // read body
	if (mp.find("Transfer-Encoding")->second == "chunked\r")
		parseChunkedBody(line, tmp);
	else if (mp.find("Content-Length") != mp.end())
	{
		std::map<string, string>::iterator it = mp.find("Content-Length");
		trim(line);
		
		if (line.length() != std::atoi(it->second.c_str()))
			throw invalid_argument("erro");
		
		tmp << line;
    }
}