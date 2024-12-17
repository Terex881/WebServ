#include "./Request.hpp"

void Request::getFileName(string &str)
{
	cout << GREEN << str << endl;
	size_t end = str.rfind("\"\r\n");
	size_t endV = str.rfind("\"; ");
	if (end != string::npos)
		str.erase(end, str.length());
	else if (endV != string::npos)
		str.erase(endV, str.length());	
}

void Request::parseBoundryBody(string &body)
{
	size_t contentStartPos = 0, filePos = 0, contentEndtPos = 0;
	string CRLF = "\r\n";
	string fileContent, fileName,rest;

	size_t boundryPos = body.find(this->boundry);
	size_t endboundryPos = body.find(this->endBoundry);
	if (boundryPos != string::npos && endboundryPos == string::npos)
	{	
		rest = body.substr(0, boundryPos);
		if (outFile.is_open()) 	outFile.write(rest.c_str(), rest.length()); outFile.flush();

		body.erase(0, boundryPos + this->boundry.length());
		// body.erase(boundryPos, this->boundry.length());
		filePos = body.rfind("filename=\"");
		if (filePos != string::npos)
		{
			if (outFile.is_open()) outFile.close();

			body.erase(0, filePos + 10);
			
			fileName = body.substr(0, body.find(CRLF) + 2); // check if filename hold "
			getFileName(fileName);
			
			contentStartPos = body.find(CRLF + CRLF) + 4;

			boundryPos = body.find(this->boundry);
			if (boundryPos != std::string::npos)
				contentEndtPos = boundryPos - contentStartPos;
			else
				contentEndtPos = body.length() - contentStartPos;
	  		if (!outFile.is_open())
				outFile.open(fileName, ios::binary);
		}
	}
	else
	{
		if (boundryPos != string::npos)
			contentEndtPos = boundryPos;
		else if (endboundryPos != std::string::npos)
			contentEndtPos = endboundryPos;
		else
			contentEndtPos = body.length();
	}

	fileContent = body.substr(contentStartPos, contentEndtPos);
	outFile.write(fileContent.c_str(), fileContent.length());
	outFile.flush();
}

void Request::parseBodyTypes(string body)
{
	if (CHUNKED)
		parseChunkedBody(body);
	else if (CHUNKED_BOUNDARY)
		parseChunkedBoundryBody(body);
	else if (BOUNDARY)
		parseBoundryBody(body);
	else
		exit(12);
}
