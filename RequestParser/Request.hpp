#pragma once
#include <cstddef>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <fstream>

using namespace std;

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define FILE_NAME "; filename=\""


class Request
{
	private:
		std::map<string, string>	mp;
		size_t						bodySize;
		static						int FINISHED;
		static						string header;
		string						boundry;
		string						endBoundry;
		static int					CHUNKED;
		static int					CHUNKED_BOUNDARY;
		static int					BOUNDARY;
		static int					CONTENT_LENGTH;
		// static string						body;

	public:
		ofstream outFile;
		Request();
		~Request();

		void	request(string &body);
		void	parseBodyTypes(string body);
		void	parseHeader(string &header);
		void	print(map<string, string> &headerMap);
		void	parseChunkedBoundryBody(string &body);
		void	parseFirstLine(string line, map<string, string> &mp);
		void	parseBoundryBody(string &body);
		void	parseChunkedBody(string &body);
		void	fillData(std::map<string, string> mp); // for fill data
		int		getFileName(string &body,  string &fileName);
		
		void 	writeFile(string &str, int start, size_t end);
		void	openFile(string fileName);
		void	isBoundary(string &body);
};