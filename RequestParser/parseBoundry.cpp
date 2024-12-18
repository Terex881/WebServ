#include "./Request.hpp"
#include <fstream>

void Request::getFileName(string &body)
{
	string first = body.substr(0, body.find(CRLF));
	body.erase(0, first.length());
	if (first.find("; filename=\"") != string::npos)
	{
		size_t end = first.rfind("\"\r\n");
		size_t endV = first.rfind("\"; ");
		if (end != string::npos)
			first.erase(end, first.length());
		else if (endV != string::npos)
			first.erase(endV, first.length());
	}
	else
	{
		string sub = body.substr(body.find(DCRLF), body.find(boundry));
		cout << sub << endl;

	}


}


size_t getEndPosition(string &body)
{
	

}


void Request::parseBoundryBody(string &body)
{
	size_t contentStartPos = 0, filePos = 0, contentEndtPos = 0;
	string fileContent, fileName, rest;

	size_t boundryPos = body.find(this->boundry);
	size_t endboundryPos = body.find(this->endBoundry);
	// while(!body.empty())
	{
		if (boundryPos == string::npos && endboundryPos == string::npos)
		{
			contentStartPos = 0;
			contentEndtPos = body.length();
		}
		if (boundryPos != string::npos)
		{
			rest = body.substr(0, boundryPos);
			if (outFile.is_open()) 	outFile.write(rest.c_str(), rest.length()); outFile.flush(); outFile.close(); // checl close here
			body.erase(0, boundryPos + boundry.length());
			
			getFileName(body);
			// body.erase(0, body.find(DCRLF));



		}
	}
	
}

// void Request::parseBoundryBody(string &body)
// {
// 	size_t contentStartPos = 0, filePos = 0, contentEndtPos = 0;
// 	string CRLF = "\r\n";
// 	string fileContent, fileName,rest;

// 	size_t boundryPos = body.find(this->boundry);
// 	size_t endboundryPos = body.find(this->endBoundry);
// 	if (boundryPos != string::npos && endboundryPos == string::npos)
// 	{	
// 		rest = body.substr(0, boundryPos);
// 		if (outFile.is_open()) 	outFile.write(rest.c_str(), rest.length()); outFile.flush();

// 		body.erase(0, boundryPos + this->boundry.length());
		
// 		// filePos = body.find("filename=\"");

// 		string firstLine = body.substr(0, body.find(CRLF));
// 		filePos = firstLine.rfind("; filename=\"");
// 		if (filePos == string::npos)
// 			storeKeyVal(body);
// 		else
// 		{
// 			if (outFile.is_open()) outFile.close();

// 			body.erase(0, filePos + 12);
			
// 			fileName = body.substr(0, body.find(CRLF) + 2); // check if filename hold "
// 			getFileName(fileName);
			
// 			contentStartPos = body.find(CRLF + CRLF) + 4;

// 			boundryPos = body.find(this->boundry);
// 			if (boundryPos != std::string::npos)
// 				contentEndtPos = boundryPos - contentStartPos;
// 			else
// 				contentEndtPos = body.length() - contentStartPos;
// 	  		if (!outFile.is_open())
				// outFile.open(fileName, ios::binary);
// 		}
// 	}
// 	else
// 	{
// 		if (boundryPos != string::npos)
// 			contentEndtPos = boundryPos;
// 		else if (endboundryPos != std::string::npos)
// 			contentEndtPos = endboundryPos;
// 		else
// 			contentEndtPos = body.length();
// 	}

// 	fileContent = body.substr(contentStartPos, contentEndtPos);
// 	outFile.write(fileContent.c_str(), fileContent.length()); outFile.flush();
// }

void Request::parseBodyTypes(string body)
{
	ofstream ss("Z.py", ios::app);
	ss << body;
	ss << "\n------------------------------------------------------------------\n";

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