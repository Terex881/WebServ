#include "./Request.hpp"
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <utility>

void	Request::writeFile(string &body, int start, size_t end, size_t len)
{
	string content = body.substr(start, end);
	if (outFile.is_open())
		outFile.write(content.c_str(), content.length()); outFile.flush();
	body.erase(0, content.length() + len);
}

void	Request::openFile(string fileName)
{
	if (!outFile.is_open()){
		outFile.open(fileName, ios::binary);
		if (!outFile.is_open())
			cout << RED << "FAILED OPEN" + fileName << endl, exit(1); // fix
	}
}

void Request::getQweryString(string &body)
{
	string key, val;
	size_t contentEndtPos  = 0, namePos = body.find("; name=\"");
	
	body.erase(0, namePos + 8);
	key = body.substr(0, body.find(CRLF) - 1);
	body.erase(0, body.find(DCRLF) + 4);
	
	if (body.find(CRLF) != std::string::npos)
		contentEndtPos = body.find(CRLF);
	else if (body.find(endBoundry) != std::string::npos)
		contentEndtPos = body.find(endBoundry);
	else
		contentEndtPos = body.length();
	
	val = body.substr(0, contentEndtPos);
	Vec.push_back(make_pair(key, val));
	body.erase(0, val.length());

	ofstream ss("X.py", ios::app);
	// ss << key << "          " << val << endl;
	// ss << "\n\n----------------------------------------------------------\n\n";
	// cout << ":" << RED << key << ":            :" << GREEN <<  val << ":" << RESET <<  endl;
	

}

int	Request::getFileName(string &body, string &fileName)
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
		return (1);
	}
	else
	{	
		getQweryString(body);
		return (0);
	}
}

void	Request::isBoundary(string &body)
{
	size_t	contentEndtPos = 0, endboundryPos = 0, boundryPos = body.find(this->boundry);
	string	fileName;

	writeFile(body, 0, boundryPos, 0); outFile.close();
	body.erase(0, boundry.length());

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
		writeFile(body, 0, contentEndtPos, 0);
	}
}

void	Request::parseBoundryBody(string &body)
{
	size_t boundryPos = body.find(this->boundry);
	size_t endboundryPos = body.find(this->endBoundry);
	
	while(!body.empty())
	{
		if (boundryPos == string::npos && endboundryPos == string::npos)
		{
			writeFile(body, 0, body.length(), 0);
		}
		if (boundryPos != string::npos)
		{
			isBoundary(body);
		}
		else if (endboundryPos != string::npos)
		{
			writeFile(body, 0, endboundryPos, endBoundry.length());
			// printV(Vec);
			REQUEST_IS_FINISH = 2;
		}
	}
}

void	Request::parseBodyTypes(string &body)
{
	ofstream ss("Z.py", ios::app);
	ss << body;
	ss << "\n\n------------------------------------------------------------------\n\n";
	
	switch (TYPE)
	{
		case (0): parseBoundryBody(body); break;
		case (1): parseChunkedBody(body); break;
		case (2): parseChunkedBoundryBody(body); break;
		default: exit(12);
	}
	
}