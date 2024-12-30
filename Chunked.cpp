#include "./Body.hpp"

void Body::parseChunkedBody(string &body)
{
	openFile("Zip/ok." + extention);
	size_t 			hexPos;
	string 			subBody;
	static u_long	length = 0;
	exit(100);

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
				/* erase last DCRLF */
				body.erase(0, 4);
				/* if strtol fails will return 0 */
				if (!body.empty())
					cout << RED << "error: strtol fails\n" << RESET;
				outFile.close();
				REQUEST_IS_FINISH = 2;
				return;
			}
		}
		subBody = body.substr(0, length);
		length -= subBody.length();
		writeFile(body, 0, subBody.length(), 0);
		// body.erase(0, subBody.length());

		/* the chunk is full and gonna read new chunk*/
		if (!length)
			body.erase(0, 2);	
	}
}

void Body::parseBodyLength(string &body)
{
	
	openFile("Zip/ok." + extention);

	// static u_long length = 0;
	bodySize -= body.length();
	if (!bodySize)
		REQUEST_IS_FINISH = 2;
	cout << RED << bodySize << endl;
	writeFile(body, 0, body.length(), 0);
}



// void Body::parseBodyLength(string &body)
// {
	
// 	openFile("Zip/ok." + extention);

// 	// static u_long length = 0;
// 	string subBody = body.substr(0, bodySize);

// 	bodySize -= subBody.length();

// 	writeFile(body, 0, body.length(), 0);
// }