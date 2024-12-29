#include "./Request.hpp"
#include <cstddef>

void Request::parseUrl(string &str)
{
	cout << YELLOW << str << endl;
	if (str.find_first_not_of("%!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
		cout << RED << "bad URL" << std::endl;
	// str = "." + str;
	size_t it;
	if ((it = str.find_first_of('\?')) != string::npos)
	{
		cout <<YELLOW << it << endl;
		queryString = str.substr(it, str.length());
		cout << GREEN << queryString << endl;
		str.erase(it);
	}
	//   /static/index.html?oli=ola



}

void Request::parseFirstLine(string &line, map<string, string> &mp)
{
	string word, key;
	int i = 0;
	// if (std::count(line.begin(),line.end(), ' ') != 2)
	// 	cout << RED << "erro: found 2 spaces in the first line" << std::endl;
	if (isspace(line[0]))
		cout << RED << "foud space in the begening";
	while(line.length() > 0)
	{
		word = line.substr(0, line.find(" ")); line.erase(0, word.length());
		key = (i == 0) ? "method" : (i == 1) ? "path" : "version";
		mp[key] = word; i++;
		line.erase(0, 1);
	}
	map<string, string>::iterator it = mp.find("method");
	if (it->second != "POST" && it->second != "GET" && it->second != "DELETE")
		cout << RED << "501 Not Implemented" << std::endl;

	// it = mp.find("path");// check other characters
	if ((it = mp.find("path")) != mp.end())
		parseUrl(it->second);

	it = mp.find("version");
	if (it->second != "HTTP/1.1")
		cout << RED << "error: invalid version" << std::endl;
}

void Request::parseHeader(string &header)
{
	string key, value;
	size_t colonPos, lineEnd; 
	size_t pos = header.find(CRLF);
	string firstLine = header.substr(0, pos);

	// check if first line has space in start and print error 
	header.erase(0, pos + 2);
	parseFirstLine(firstLine, mp);

	while(!header.empty())
	{
		colonPos = header.find(":");
		if(colonPos == string::npos || std::isspace(header[colonPos - 1]) || std:: isspace(header[0]))
		{
			cout << RED << "foud space or doesn't find :";
			break;
		}
		lineEnd = header.find(CRLF);
		if(lineEnd == string::npos)
			lineEnd = header.length();
		key = header.substr(0, colonPos);
		key.erase(0, key.find_first_not_of(" "));

		value = header.substr(colonPos + 1, lineEnd - colonPos - 1);
		value.erase(0, value.find_first_not_of(" "));

      	std::transform(key.begin(), key.end(), key.begin(), ::tolower); // check this 
		if (key == "transfer-encoding") // check other 
        	std::transform(value.begin(), value.end(), value.begin(), ::tolower); // check this 
		
		mp[key] = value;
		
		header.erase(0, lineEnd + 2);
	}
	print(mp);
	fillData(mp);
}


const string Request::getExtention(std::map<string, string> mp)
{
	std::map<string, string>::iterator it = mp.find("content-type");
	if (it != mp.end())
	{
		string str = it->second.substr(it->second.find("/") + 1, it->second.length());
		if(str == "octet-stream")
			return "py";
		return str;
	}
	else
		cout << RED << "Bad request\n" ;
	return ""; // check this 
	// GET NAME FROM TIME
}


void Request::fillData(const std::map<string, string> &mp)
{
	REQUEST_IS_FINISH = 1;

	map<string, string>::const_iterator	lengthPos = mp.find("content-length");
	map<string, string>::const_iterator	boundry = mp.find("content-type");
	map<string, string>::const_iterator	chunked = mp.find("transfer-encoding");
	
	if (lengthPos != mp.end())
		bodySize = std::atol(lengthPos->second.c_str());
	if (mp.find("host") == mp.end())
		cout << RED << "no Host found !!\n" << RESET;

	if (boundry != mp.end())
		extention = getExtention(mp);
	else
		cout << RED << "no type founded" << endl;

	
	
	TYPE = 3;
	
	bool bol = boundry != mp.end() && boundry->second.find("multipart/form-data;") != std::string::npos;
	if (bol)
	{
		string sep = boundry->second.substr(boundry->second.rfind("boundary=") + 9 , boundry->second.length());
		this->boundry		= "--" + sep + "\r\n";
		this->endBoundry	= "\r\n--" + sep + "--\r\n";
		TYPE = 0;
	}
	if (chunked != mp.end())
	{
		if (chunked->second != "chunked")
			cout << RED << "not implemented\n" << RESET ;
		else if (chunked->second == "chunked" && !bol)
			TYPE = 1;
		else if (chunked->second == "chunked" && bol)
		{
			TYPE = 2;
		}
	}
	if (!TEST.is_open())
	{
		TEST.open("Z.py", ios::app|ios::binary);
	}
}