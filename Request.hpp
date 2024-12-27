#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define FILE_NAME "; filename=\""

using namespace std;

class Request  
{
	private:
		string 									extention;
		string									boundry;
		string									endBoundry;
		std::map<string, string>				mp;
		size_t									bodySize;
		int										REQUEST_IS_FINISH;
		int										TYPE;
		string									header;
		ofstream								outFile;
		ofstream								TEST;

		string newStr;
		

	public:
		Request();
		void			parseHeader(string &header);
		void			fillData(const std::map<string, string> &mp); // for fill data
		const string	getExtention(std::map<string, string> mp);

		void			request(string &body);
		void			print(map<string, string> &headerMap);
		void			printV(vector<pair<string, string> > &mp);
		int				getFileName(string &body,  string &fileName);
		void 			writeFile(string &body, int start, size_t end, size_t len);
		void			openFile(string fileName);
		bool			isBoundary(string &body);
		void			getQweryString(string &body);
		int				getStat() const;

		void			parseFirstLine(string line, map<string, string> &mp);
		void			parseBodyTypes(string &body);
		void			parseBoundryBody(string &body);
		void			parseChunkedBody(string &body);
		void			parseBodyLength(string &body);
		void			parseChunkedBoundryBody(string &body);
};