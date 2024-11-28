#include "Request.hpp"
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
	str = str.substr(first, last - first + 1);
	// cout <<"---->:" <<  str << endl;
}

void parseChunkedBody(string line)
{
	char *end; string res;
    while (1337)
	{
		long l = strtol(line.c_str(), &end, 16);
		if (l == 0)
			break;
		line = end;
		trim(line);

		res = line.substr(0, l);
		cout << l << "   " << res.length() << endl;
		if (l != res.length())
			throw invalid_argument("error");


		line.erase(0, l + 2);
		trim(line);
    }
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
	if (mp.find("Transfer-Encoding")->second == "chunked\r")
		parseChunkedBody(line);
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