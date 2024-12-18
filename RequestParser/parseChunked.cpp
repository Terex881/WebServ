#include "./Request.hpp"


void Request::parseChunkedBody(string &body)
{
	std::ofstream tmp("OK.py", ios::app);
	string res;
	size_t hexPos;
	static unsigned long length;

	size_t last  = body.rfind("\r\n\r\n");
	if (last != string::npos)
		body.erase(last, 4);

	// while (1337)
	{
		hexPos = body.find_first_not_of("0123456789abcdefABCDEF");
		if (hexPos == string::npos)	 return;;
		length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
		
		body.erase(0, hexPos + 2);
		if (length == 0)
		{
			// if (!body.empty())
			// 	cout << RED << "error: strtol doesn't found an hexadecimal return 0" << std::endl; // if the length not valid strtol fails return 0
			return;
		}
		res = body.substr(0, length);
		// if (length != res.length())
		// 	cout << RED << "error: the length of the chunked data dosn't match" << std::endl;

		tmp.write(body.c_str(), body.length()); tmp.flush();
		body.erase(0, length);
		// if (body[0] != '\r' && body[1] != '\n')
			// cout << RED << "error: found 'r n' inside body" << std::endl; // ckech if it fails to close the \nfile
		body.erase(0, 2);
	} // check to make it 
}

void Request::parseChunkedBoundryBody(string &body)
{
	string res;
	size_t hexPos;
	unsigned long length;

	while (1337)
	{
		hexPos = body.find_first_not_of("0123456789abcdefABCDEF");
		if (hexPos == string::npos) break;
		
		length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);

		body.erase(0, hexPos + 2);
		if (length == 0)
			break;
		res.append(body.substr(0, length));
		body.erase(0, length + 2);
	}
	parseBoundryBody(res);
}