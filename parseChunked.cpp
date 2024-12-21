#include "./Request.hpp"



void Request::parseChunkedBoundryBody(string &body)
{
		// openFile("ok." + extention);
	size_t hexPos;
	string subBody;
	static string res;
	ofstream ss("X.py", ios::app);


	while(!body.empty())
	{
		if (length == 0)
		{
			hexPos = body.find_first_not_of("0123456789abcdefABCDEF");	
			if (hexPos == string::npos)
				cout << RED << "error: no length founded\n" << RESET;
			// exit(1);
			length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
			body.erase(0, hexPos + 2);
			cout << YELLOW << length << endl;

			if (!length)
			{
				ss << body;
				
				ss << "\n---------------------------------------\n";
				body.erase(0, 4);
				
				if (!body.empty())
					cout << RED << "error: strtol fails\n" << RESET;
				return;
			}
		}
		subBody = body.substr(0, length);
		length -= subBody.length();
		
		parseBoundryBody(subBody);
		body.erase(0, subBody.length());


		/* the chunk is full and gonna read new chunk*/
		if (!length)
		{
			body.erase(0, 2);	
			exit(3);
		}
		exit(1);
	}

}