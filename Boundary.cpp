#include "./Request.hpp"

void	Request::writeFile(string &body, int start, size_t end, size_t len)
{
	string content = body.substr(start, end);
	if (outFile.is_open())
		outFile.write(content.c_str(), content.length()); outFile.flush();
	body.erase(0, end + len);
}

void	Request::openFile(string fileName)
{
	if (!outFile.is_open()){
		outFile.open(fileName, ios::binary | ios::app);
		if (!outFile.is_open())
			cout << RED << "FAILED OPEN" + fileName << endl, exit(1); // fix
	}
}

void	Request::getQweryString(string &body)
{
	string subBody;
	size_t contentEndtPos  = 0;
	body.erase(0, body.find(DCRLF) + 4);

	size_t boundryPos = body.find(this->boundry);
	size_t endboundryPos = body.find(this->endBoundry); // chnage

	if (boundryPos != std::string::npos)
		contentEndtPos = boundryPos;
	else if (endboundryPos != std::string::npos)
		contentEndtPos = endboundryPos;
	else
		contentEndtPos = body.length();

	// subBody = body.substr(0, contentEndtPos);

	static int i = 0;
	openFile(".Zip/File" + to_string(i) + ".txt");	i++;
	writeFile(body, 0, contentEndtPos, 0);
}

int	Request::getFileName(string &body, string &fileName)
{
	string tmp = body.substr(body.find(boundry) + boundry.length(), body.length());
	string first = tmp.substr(0, tmp.find(CRLF) + 2);

	if (first.find("\"\r\n") == string::npos)
	{
		cout << RED << "error: no CRLF founded\n" << RESET;
		return 0;
	}
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
		getQweryString(body);
	return -1;
}

bool	Request::isBoundary(string &body)
{
	size_t	contentEndtPos = 0, endboundryPos = 0, boundryPos = body.find(this->boundry);
	string	fileName;

	if (boundryPos != 0) writeFile(body, 0, boundryPos - 2, 2);
	else writeFile(body, 0, boundryPos, 0);
	outFile.close();
	
	int i = getFileName(body, fileName);
	if (i == 1)
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

		openFile(".Zip/" + fileName);
		writeFile(body, 0, contentEndtPos, 0);
	}
	else if (!i)
		return 0;
	return 1;
}
// 6.3.45

void	Request::parseBoundryBody(string &body)
{
	size_t boundryPos, endboundryPos;
		endboundryPos = body.find(endBoundry);
	while(!body.empty())
	{
		boundryPos = body.find(boundry);
		if (boundryPos == string::npos && endboundryPos == string::npos)
		{
			if (body == CRLF && TYPE == 2) body.erase(0, 2);
			else writeFile(body, 0, body.length(), 0);
		}
		if (boundryPos != string::npos)
		{
			if (!isBoundary(body)) return;
		}
		else if (endboundryPos != string::npos)
		{
			endboundryPos = body.find(endBoundry);	
			writeFile(body, 0, endboundryPos, endBoundry.length());
			REQUEST_IS_FINISH = 2;
		}
	}
}