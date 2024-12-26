#include "Request.hpp"

void Request::parseChunkedBoundryBody(string &body)
{
	size_t hexPos;
	string subBody;
	static u_long	length = 0;
	while(!body.empty())
	{
		if (length == 0)		
		{

			hexPos = body.find_first_not_of("0123456789abcdefABCDEF");	
			if (hexPos == string::npos)
			{
				cout << RED << "error: no length founded\n" << RESET;
				return;
			}
			length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
			body.erase(0, hexPos + 2);

			if (!length)
			{
				body.erase(0, 4);
				return;
			}
		}
		subBody = body.substr(0, length);
		length -= subBody.length();

		body.erase(0, subBody.length());
		parseBoundryBody(subBody);
		if (!length)
		{
			body.erase(0, 2);
		}
	}
}