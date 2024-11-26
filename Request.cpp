#include "Request.hpp"
#include <cstdlib>

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

void parseChunkedBody(string line, map<string, string> mp)
{
	char *end; // check if i need to parse with end 
	long lenght = std::strtol(line.c_str(), &end, 16);
	cout << lenght << endl;
}

void parseRequest(string str)
{
	std::map<string, string> mp;
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

	getline(ss, line, '\0');
	cout << line << endl;
	// if (mp.find("Transfer-Encoding")->second == "chunked\r")
	// 	parseChunkedBody(line, mp);
    // else
    // {
	//     std::map<string, string>::iterator it = mp.find("Content-Length"); 
	//     // cout << it->first << "    " << it->second << endl;
	//     if (line.length() != stoi(it->second))
	//     	throw invalid_argument("erro");
    // }
	// for(std::map<string, string>::iterator it = mp.begin(); it != mp.end(); it++)
	// 	cout << it->first << "    " << it->second << "1" << endl;
}
