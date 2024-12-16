#include "./Request.hpp"

void Request::parseBoundryBody(string &body)
{
	size_t contentStartPos = 0, filePos = 0, contentEndtPos = 0;
	string CRLF = "\r\n";
	string fileContent, fileName;

	ofstream ss("xx.py", ios::app);

	ss << body;
	ss << "\n-------------------------------------------\n";

	size_t boundryPos = body.find(this->boundry);
	size_t endboundryPos = body.find(this->endBoundry);
	
	if (boundryPos != string::npos && endboundryPos == string::npos)
	{
		cout << RED << "OK" << RESET << endl;
		/* chheck to remove the endBoundry from this condition*/
		
		string rest = body.substr(0, boundryPos - 2);
		if (outFile.is_open())
			outFile << rest;


		body.erase(boundryPos, this->boundry.length() + 2);
		filePos = body.find("filename=\"");
		if (filePos != string::npos)
		{
			body.erase(0, filePos + 10);
			fileName = body.substr(0, body.find_first_of("\"")); // check if filename hold "
			
			contentStartPos = body.find(CRLF + CRLF) + 4;
			boundryPos = body.find(this->boundry);
			if (boundryPos != std::string::npos)
				contentEndtPos = boundryPos - contentStartPos;
			else
				contentEndtPos = body.length() - contentStartPos;
	  		if (!outFile.is_open())
				outFile.open(fileName);
		}
	}
	else
	{
		// cout << GREEN << "OK" << RESET << endl;
		contentStartPos = 0;
		endboundryPos = body.find(this->endBoundry);
		if (endboundryPos != std::string::npos)
			contentEndtPos = endboundryPos - contentStartPos;
		else
			contentEndtPos = body.length() - contentStartPos;
	}
	fileContent = body.substr(contentStartPos, contentEndtPos);

	outFile << fileContent;
	outFile.flush();
}



void Request::parseBodyTypes(string body, map<string, string> mp)
{
	// rje3 zmer miniscule
	// 3awd checkiy dak first line
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
		this->boundry = "--" + boundry->second.substr(boundry->second.find("=") + 1,\
			boundry->second.find("\r") - boundry->second.find("=") - 1);

		this->endBoundry = "\r\n" + this->boundry + "--";
		parseBoundryBody(body);
		
	}
	// else
	// 	exit(12); //content length
}
