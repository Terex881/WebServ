#include "Body.hpp"
#include <fstream>

ofstream Body::outFile;

void	Body::parseBodyTypes(string &body)
{
	newStr.append(body.c_str(), body.length());

	// cout << BLUE << "in body:" << atay_tkhwa->getType() << RESET << endl;


	switch (atay_tkhwa->getType())
	{
		case (0):parseBoundryBody(newStr); break;
		case (1): parseChunkedBody(newStr); break;
		case (2): parseChunkedBoundryBody(newStr); break;
		case (3): parseBodyLength(newStr); break;
	}
}