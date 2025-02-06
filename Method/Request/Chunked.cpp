#include "Request.hpp"

string _to_string(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

string getNameFromTime()
{
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);
	size_t ms = localTime->tm_sec * 1000;
	return (_to_string(localTime->tm_hour) + ":" + _to_string(localTime->tm_min) + ":" + _to_string(localTime->tm_sec) + ":" + _to_string(ms));
}

void Request::parseChunkedBody(string &body)
{
	RequestData.isUpload = true;
	BodyData.pathFormData = RequestData.fileLocation + "/" + getNameFromTime();

	openFile(BodyData.pathFormData);

	size_t hexPos, strPos;
	string subBody, str;
	static u_long	length = 0;

	while(!body.empty())
	{
		if (length == 0)
		{
			strPos = body.find_first_of("0123456789abcdefABCDEF");
			if (strPos != 0)
			{
				str = body.substr(0, strPos);
				body.erase(0, str.length());
				if (str != CRLF && str.find_first_of(CRLF) == NP)
					writeFile(str, 0, str.length(), 0);
			}
			hexPos = body.find_first_not_of("0123456789abcdefABCDEF");	
			if (hexPos == NP)
				return;
			
			length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
			body.erase(0, hexPos + 2);

			if (!length)
			{
				body.erase(0, 4);
				outFile.close();
				RequestData.requestStat = 2;
				RequestData.codeStatus = 201;
				return;
			}
		}
		subBody = body.substr(0, length);
		length -= subBody.length();
		writeFile(body, 0, subBody.length(), 0);
	}
}

void Request::parseBodyLength(string &body)
{
	RequestData.isUpload = true;
	BodyData.pathFormData = RequestData.fileLocation + "/" + getNameFromTime();
	openFile(BodyData.pathFormData);
	
	size_t tmp = BodyData.bodySize;
	tmp -= body.length();
	BodyData.bodySize = tmp;
	writeFile(body, 0, body.length(), 0);
	if (!BodyData.bodySize)
	{
		RequestData.requestStat = 2;
		RequestData.codeStatus = 201;
		outFile.close();
	}
}