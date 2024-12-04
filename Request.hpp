
#pragma once
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <fstream>


using namespace std;


void parseRequest(string str);


class Request
{
    private:
    	std::map<string, string> headerMap;

    public:
        Request(string body);

};
// add const 



// void parseBoundryBody(string line, map<string, string> headerMap)
// {
// 		map<string, string>::iterator it = headerMap.find("Content-Type");
// 		string title, boundry, delimter, data, subline;
// 		int filePos; int namePos;
// 		it = headerMap.find("Content-Type");
// 		title = it->second.substr(0, it->second.find(";"));
// 		boundry = it->second.substr(it->second.find("=") + 1, it->second.find("\r") - it->second.find("=") - 1);
// 		delimter = "--" + boundry;
		
// 		while(!line.empty())
// 		{	
// 			line = line.c_str() + delimter.length();
// 			trim(line); // remove \r\n from begining and end of line
// 			subline = line.substr(0, line.find("\n")); // get line by line
// 			size_t sublineSize = subline.length();
// 			namePos = subline.find("name=\"");
// 			if (namePos != std::string::npos)
// 			{
// 				subline.erase(0, namePos + 6); // add 6 to skip --> name=\"
// 				string name = subline.substr(0, subline.find("\";")); 
// 				// cout << name << endl;
// 			}
// 			filePos = subline.find("filename=\"");
// 			if (filePos != string::npos)
// 			{
// 				subline.erase(0, filePos + 10);
// 				string fileName = subline.substr(0, subline.find("\"\r"));
// 				// cout << file << endl;
// 				ofstream file(fileName);
// 			}
// 			line.erase(0, sublineSize); trim(line);
// 		}

// }