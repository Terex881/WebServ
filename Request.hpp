#pragma once


// #include "Header.hpp"
#include <atomic>
#include <cstddef>
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
		string extention;
		string									boundry;
		string									endBoundry;
		std::vector<std::pair<string, string> >	Vec;
		std::map<string, string>				mp;
		size_t									bodySize;
		static int								REQUEST_IS_FINISH;
		static int								TYPE;
		
		string									header;
		ofstream								outFile;
		static unsigned long	length;
		static unsigned long	restlength;
		
		static int								IF_FILENAME;
		string salah;
		string newStr;
		

	public:
		Request();
		void			parseFirstLine(string line, map<string, string> &mp);
		void			parseHeader(string &header);
		void			fillData(const std::map<string, string> &mp); // for fill data
		const string	getExtention(std::map<string, string> mp);

		void			request(string &body);
		void			parseBodyTypes(string &body);
		void			print(map<string, string> &headerMap);
		void			printV(vector<pair<string, string> > &mp);
		void			parseChunkedBoundryBody(string &body);
		void			parseBoundryBody(string &body);
		void			parseBoundryBodyA(string &body);
		void			parseChunkedBody(string &body);
		int				getFileName(string &body,  string &fileName);
		int				getFileNameA(string &body,  string &fileName);
		void 			writeFile(string &body, int start, size_t end, size_t len);
		void 			writeFileA(string &body, int start, size_t end, size_t len);
		void			openFile(string fileName);
		void			openFileA(string fileName);
		void			isBoundary(string &body);
		void			isBoundaryA(string &body);
		void			getQweryString(string &body);
		void			getQweryStringA(string &body);

		int				getStat() const;
};