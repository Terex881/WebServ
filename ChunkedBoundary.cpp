#include "Request.hpp"

void Request::parseChunkedBoundryBody(string &body)
{
	size_t hexPos, strPos;
	string subBody, str;
	static u_long	length = 0;
	while(!body.empty())
	{
		if (length == 0)		
		{	
			strPos = body.find_first_of("0123456789abcdefABCDEF");	
			/* check if not 0 to avoid reallocat for nothing */
			if (strPos != 0)
			{
				/* get string before hexadecimal and erase it and write the str to file*/
				str = body.substr(0, strPos);
				body.erase(0, str.length());;
				if (str != CRLF)
					parseBoundryBody(str);
			}
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
				body.erase(0, 4);return;
			}
		}
		subBody = body.substr(0, length);
		length -= subBody.length();

		body.erase(0, subBody.length());
		parseBoundryBody(subBody);
		// if (!length)
		// {
		// 	body.erase(0, 2);
		// }
	}
}