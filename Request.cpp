#include "Request.hpp"

int Request::isFinish = 0;
string Request::header;
string Request::body;

Request::Request()
{
	bodySize = 0;
	isFinish = 0;
}

void Request::print(map<string, string> &mp)
{
	for(map<string, string>::iterator it = mp.begin(); it!=mp.end(); it++)
		cout << RED << ":" << it->first << ": :" << GREEN << it->second << ":" << endl;

}

void Request::parseFirstLine(string line, map<string, string> &mp)
{
	string word;
	string key;
	int i = 0;
	if (std::count(line.begin(),line.end(), ' ') != 2)
		cout << RED << "erro: found 2 spaces in the first line" << std::endl;

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

	it = mp.find("path");// check other characters
	if (it->second.find_first_not_of("%!#$&'()*+,/:;=?@[]-_.~ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") != std::string::npos)
		cout << RED << "bad URL" << std::endl;

	it = mp.find("version");
	if (it->second != "HTTP/1.1")
		cout << RED << "error: invalid version" << std::endl;
}

void Request::parseChunkedBody(string &body)
{
	std::ofstream tmp("OK.py");
	string res;
	size_t hexPos;
	unsigned long length;

	size_t last  = body.rfind("\r\n\r\n");
	if (last != string::npos)
		body.erase(last, 4);

	while (1337)
	{
		hexPos = body.find_first_not_of("0123456789abcdefABCDEF");
		if (hexPos == string::npos)	 break;
		length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
		
		body.erase(0, hexPos + 2);
		if (length == 0)
		{
			if (!body.empty())
				cout << RED << "error: strtol doesn't found an hexadecimal return 0" << std::endl; // if the length not valid strtol fails return 0
			break;
		}
		res = body.substr(0, length);
		if (length != res.length())
			cout << RED << "error: the length of the chunked data dosn't match" << std::endl;

		body.erase(0, length);
		tmp.write(res.c_str(), res.length());
		if (body[0] != '\r' && body[1] != '\n')
			cout << RED << "error: found 'r n' inside body" << std::endl; // ckech if it fails to close the \nfile
		body.erase(0, 2);
	} // check to make it 
}

void Request::parseHeader(string &header)
{
	string key, value;
	size_t colonPos, lineEnd; 
	size_t pos = header.find("\r\n");
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
		lineEnd = header.find("\r\n");
		if(lineEnd == string::npos)
			lineEnd = header.length();
		key = header.substr(0, colonPos);
		key.erase(0, key.find_first_not_of(" "));

		value = header.substr(colonPos + 1, lineEnd - colonPos - 1);
		value.erase(0, value.find_first_not_of(" "));

		mp[key] = value;
		
		header.erase(0, lineEnd + 2);
	}
	map<string, string>::iterator lengthPos = mp.find("Content-Length");
	if (lengthPos != mp.end())
		bodySize = std::atol(lengthPos->second.c_str());
	if(mp.find("Host") == mp.end())
		cout << RED << "no Host found !!\n" << RESET;

	// print(mp);
	isFinish = 1;
}

void Request::parseBoundryBody(string &body)
{
	ofstream ss("ttt.py", ios::app);
	ss << body;
	ss << "\n\n-----------------------------------------------\n\n";
	string fileContent, fileName;
	size_t contentStartPos, filePos;
	string CRLF = "\r\n";
	string seperator = body.substr(0, body.find_first_of(CRLF));
	
	while (!body.empty())
	{
		body.erase(0, seperator.length() + 2);
		
		if (body.find(seperator) == string::npos) 
			break;
		
		filePos = body.find("filename=\"");
		if (filePos != string::npos)
		{
			body.erase(0, filePos + 10);
			fileName = body.substr(0, body.find_first_of("\"")); // check if filename hold "
			
			contentStartPos = body.find(CRLF + CRLF);
			if (contentStartPos != string::npos)
			{
				fileContent = body.substr(contentStartPos + 4, body.find(seperator) - contentStartPos - 4 - 2);
				ofstream outFile(fileName, ios::binary);
				if (outFile.is_open())
					outFile << fileContent; // check if open fails
			}
		}
		body.erase(0, body.find(seperator));
	}
}

void Request::parseChunkedBoundryBody(string &body)
{
	string res;
	size_t hexPos;
	unsigned long length;

	while (1337)
	{
		hexPos = body.find_first_not_of("0123456789abcdefABCDEF");
		if (hexPos == string::npos) break;
		
		length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);

		body.erase(0, hexPos + 2);
		if (length == 0)
			break;
		res.append(body.substr(0, length));
		body.erase(0, length + 2);
	}
	parseBoundryBody(res);
}


void Request::parseBodyTypes(string body, map<string, string> mp)
{
	// rje3 zmer miniscule
	map<string, string>::iterator chunked = mp.find("Transfer-Encoding");
	map<string, string>::iterator boundry = mp.find("Content-Type");
	int i = boundry != mp.end() && boundry->second.find("multipart/form-data;") != std::string::npos;

	if (chunked != mp.end())
	{
		if (chunked->second != "chunked")
			cout << RED << "not implemented\n" ;
		else if (chunked->second == "chunked" && !i)
			parseChunkedBody(body);
		else if (chunked->second == "chunked" && i)
			parseChunkedBoundryBody(body);
	}
	else if (i)
	{
		parseBoundryBody(body);
	}
	else
		exit(12); //content length
}

void Request::request(string &request)
{
	if (!isFinish)
	{
		size_t pos = request.find("\r\n\r\n");
		if (pos != string::npos)
		{
			header += request.substr(0, pos);
			request.erase(0, pos + 4);
			parseHeader(header);
		}
		else
			header.append(request);
	}
	if (isFinish == 1)
	{
		body.append(request);
		parseBodyTypes(body, mp);
	}
}