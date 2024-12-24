#include "Request.hpp"


void	Request::writeFileA(string &body, int start, size_t end, size_t len)
{
	string content = body.substr(start, end);

	if (outFile.is_open())
	{
		outFile.write(content.c_str(), content.length()); outFile.flush();	
	}
	
	body.erase(0, end + len);
	// body.erase(0, content.length() + len);
}

void	Request::getQweryStringA(string &body, int flag)
{
	string key, val;
	size_t contentEndtPos = 0, namePos = body.find("; name=\"");
	
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
	
}

void	Request::openFileA(string fileName)
{
	if (!outFile.is_open()){
		outFile.open(fileName, ios::binary);
		if (!outFile.is_open())
			cout << RED << "FAILED OPEN" + fileName << endl, exit(1); // fix
	}
}

int	Request::getFileNameA(string &body, string &fileName)
{
	string tmp = body.substr(body.find(boundry) + boundry.length(), body.length());

	string first = tmp.substr(0, tmp.find(CRLF) + 2);
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
		if (body.find(boundry) == string::npos || body.find(endBoundry) == string::npos)
		{
			IF_FILENAME = 33;
			
			throw "HELLO";
		}
		getQweryStringA(body, 1);
		return (0);
	}
}

void	Request::isBoundaryA(string &body)
{
	size_t	contentEndtPos = 0, endboundryPos = 0, boundryPos = body.find(this->boundry);
	string	fileName;
	
	writeFileA(body, 0, boundryPos , 0); outFile.close();
	
	// string tmp = body.substr(boundryPos, body.length());
	
	if (getFileNameA(body, fileName))
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

		openFileA(fileName);
		writeFileA(body, 0, contentEndtPos, 0);
	}
	body.erase(0, boundry.length());
	
}

void	Request::parseBoundryBodyA(string &body)
{
	salah += body;
	
	size_t boundryPos = salah.find(this->boundry);
	size_t endboundryPos = salah.find(this->endBoundry);
	try {
		while(!salah.empty())
		{
			if (IF_FILENAME == 33)
			{
				getQweryStringA(salah, 0);
				IF_FILENAME = 0;
				return;              
			}
			if (boundryPos == string::npos && endboundryPos == string::npos)
			{
				if (salah == CRLF && TYPE == 2) salah.erase(0, 2);
				else writeFileA(salah, 0, salah.length(), 0);
			}
			if (boundryPos != string::npos)
			{
				isBoundaryA(salah);
			}
			if (endboundryPos != string::npos)
			{
				writeFileA(salah, 0, endboundryPos, salah.length());
				printV(Vec);
				REQUEST_IS_FINISH = 2;
			}
		}
	} catch (const char *c)
	{
		return;
	};

}

void Request::parseChunkedBoundryBody(string &body)
{
	size_t hexPos;
	string subBody;

	while(!body.empty())
	{
		if (length == 0)		
		{

			hexPos = body.find_first_not_of("0123456789abcdefABCDEF");	
			if (hexPos == string::npos)
				return;
			length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
			body.erase(0, hexPos + 2);

			if (!length)
			{
				body.erase(0, 4);
				return;
			}
		}

		subBody = body.substr(0, length);
		
		// cout << RED << length << " - " << subBody.length() << " = " << RESET;
		length -= subBody.length();
		// cout << GREEN << length << RESET << endl;
		
		// cout << "before body length is: " << body.length() << endl;
		body.erase(0, subBody.length());
		parseBoundryBodyA(subBody);
		// cout << "after body length is: " << body.length() << endl;
		if (!length)
		{
			body.erase(0, 2);
		}
	}
}