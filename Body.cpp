#include "Body.hpp"

void	Body::parseBodyTypes(string &body, Request *r)
{
	newStr.append(body.c_str(), body.length());

	// cout << BLUE << "in body:" << r->getType() << RESET << endl;

	switch (r->getType())
	{
		case (0):parseBoundryBody(newStr); break;
		case (1): parseChunkedBody(newStr); break;
		case (2): parseChunkedBoundryBody(newStr); break;
		case (3): parseBodyLength(newStr); break;
	}
}