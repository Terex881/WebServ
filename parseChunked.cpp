#include "./Request.hpp"



void Request::parseChunkedBoundryBody(string &body)
{
	size_t hexPos;
	string subBody;
	// ofstream ss("X.py", ios::app);
	// ofstream dd("Y.py", ios::app);


	while(!body.empty())
	{
		if (length == 0)		
		{

			hexPos = body.find_first_not_of("0123456789abcdefABCDEF");	
			if (hexPos == string::npos)
				cout << RED << "error: no length founded\n" << RESET;
			
			
			// cout << YELLOW << body.substr(0, hexPos) << endl;
			length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
			body.erase(0, hexPos + 2);

			if (!length)
			{
				body.erase(0, 4);
				return;
			}
		}

		subBody = body.substr(0, length);
		
		cout << RED << length << " - " << subBody.length() << " = " << RESET;
		length -= subBody.length();
		cout << GREEN << length << RESET << endl;
		
		// cout << "before body length is: " << body.length() << endl;
		body.erase(0, subBody.length());
		parseBoundryBody(subBody);
		// cout << "after body length is: " << body.length() << endl;

		if (!length)
		{
			body.erase(0, 2);
		}
		
	}



}