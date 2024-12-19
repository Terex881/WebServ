#include "./Request.hpp"
#include <fstream>

void Request::writeFile(string &body, int start, size_t end)
{
	string content = body.substr(start, end);
	if (outFile.is_open())
		outFile.write(content.c_str(), content.length()); outFile.flush();
	body.erase(0, content.length());
}

void Request::openFile(string fileName)
{
	if (!outFile.is_open()){
		outFile.open(fileName, ios::binary);
		if (!outFile.is_open())
			cout << RED << "FAILED OPEN" + fileName << endl, exit(1); // fix
	}
}

int Request::getFileName(string &body, string &fileName)
{
	string first = body.substr(0, body.find(CRLF) + 2);
	size_t filePos = first.find(FILE_NAME);

	if (filePos != std::string::npos)
	{
		first.erase(0, filePos + 12);
		size_t end = first.rfind("\"\r\n");
		size_t endV = first.rfind("\"; ");
		if (end != string::npos)
			fileName = first.substr(0, end);
		else if (endV != string::npos)
			fileName = first.substr(0, endV);
		cout << RED << fileName << endl;
		return (1);
	}
	else
	{
		// check multi and boundri
		// ofstream ss("X.py", ios::app);
		// ss << sub << endl;
		// ss << "\n\n---------------------\n\n";
		
		string sub  = body.substr(0, body.find(boundry));
		body.erase(0, sub.length());
		return (0);
	}
}
void Request::isBoundary(string &body)
{
	ofstream ss("OK.py", ios::app);
	size_t filePos = 0, contentEndtPos = 0, endboundryPos = 0, boundryPos = body.find(this->boundry);
	string fileContent, fileName, rest;

	writeFile(body, 0, boundryPos); outFile.close();

	
	body.erase(0, boundryPos + boundry.length());
	
	if (getFileName(body, fileName))
	{
		body.erase(0, body.find(DCRLF) + 4);

		boundryPos = body.find(this->boundry);
		endboundryPos = body.find(this->endBoundry);

		if (boundryPos != std::string::npos)
			contentEndtPos = boundryPos;
		else if ( endboundryPos != std::string::npos )
			contentEndtPos = endboundryPos;
		else
			contentEndtPos = body.length();

		openFile(fileName);
		writeFile(body, 0, contentEndtPos);
	}
}

void Request::parseBoundryBody(string &body)
{
	size_t boundryPos = body.find(this->boundry);
	size_t endboundryPos = body.find(this->endBoundry);
	
	while(!body.empty())
	{
		if (boundryPos == string::npos && endboundryPos == string::npos)
		{
			writeFile(body, 0, body.length());
		}
		if (boundryPos != string::npos)
		{
			isBoundary(body);
		}
		else if (endboundryPos != string::npos)
		{
			writeFile(body, 0, endboundryPos);
		}
		cout << "__\n";
	}
}

void Request::parseBodyTypes(string body)
{
	ofstream ss("Z.py", ios::app);
	ss << body;
	ss << "\n\n------------------------------------------------------------------\n\n";

	if (CHUNKED)
		parseChunkedBody(body);
	else if (CHUNKED_BOUNDARY)
	{
		parseChunkedBoundryBody(body);
		exit(1);
	}
	else if (BOUNDARY)
	{
		parseBoundryBody(body);
		// exit(1);
	}
	else
		exit(12);
}