#pragma once

#include "Request.hpp"

class Body :  public Request
{
	private:
		static ofstream outFile;
		std::vector<std::pair<string, string> > Vec;
		string newStr;


		void openFile(string fileName);
		bool isBoundary(string &body);
		void writeFile(string &body, int start, size_t end, size_t len);
		void getQweryString(string &body);
		void parseBoundryBody(string &body);
		void parseChunkedBody(string &body);
		void parseBodyLength(string &body);
		void parseChunkedBoundryBody(string &body);
		int getFileName(string &body, string &fileName);

	public:
		void parseBodyTypes(string &body);
		Request *atay_tkhwa;
};
