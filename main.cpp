
#include <exception>
#include <iostream>
#include <sstream>
#include <map>
#include <stdexcept>
#include <string>

using namespace std;


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


void parseRequest(string str)
{
	std::map<string, string> mp;
	string line, key, val;
	stringstream ss(str);
	getline(ss, line, '\n');
	getFirstLine(line, mp); // store first line in the map
	while(getline(ss, line, '\n'))
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
	
	for(std::map<string, string>::iterator it = mp.begin(); it != mp.end(); it++)
	{
		cout << it->first << "      " << it->second<< endl;
	}
}

int main()
{
	try {
		string str= "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
						"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
						"Host: www.tutorialspoint.com\r\n"
						"Content-Type: application/x-www-form-urlencoded\r\n"
						"Content-Length: length\r\n"
						"Accept-Language: en-us\r\n"
						"Accept-Encoding: gzip, deflate\r\n"
						"Connection: Keep-Alive\r\n";
						
		string str1= 	"GET /info.html HTTP/1.1\r\n"
						"Host: localhost:8080\r\n"
						"Connection: keep-alive\r\n"
						"Cache-Control: max-age=0\r\n"
						"Upgrade-Insecure-Requests: 1\r\n"
						"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.162 Safari/537.36\r\n"
						"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
						"DNT: 1\r\n"
						"Accept-Encoding: gzip, deflate, br\r\n"
						"Accept-Language: en-US,en;q=0.9\r\n";
		parseRequest(str1);	
	} catch (const std::exception &e) {
		cout << e.what() << " ----" << endl;
	}
	return 0;
}