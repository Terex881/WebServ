#pragma once

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "sstream"


#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define FILE_NAME "; filename=\""
#define NP std::string::npos

using std::vector;
using std::map;
using std::string;
using std::make_pair;

#include "../../Config/File_Parsing.hpp"

enum type
{
	BOUNDARY,
	CHUNKED,
	CHUNKED_BOUNDARY,
	BODY_SIZE,
	NONE,
};

typedef struct s_Request
{
	string						header;
	int							requestStat;
	int 						fd;
	int							is_server;
	int							is_client;
	int 						sent_head;
	string						first;
	size_t						bytes_sent;
	std::ifstream				*file;
	int							codeStatus;
	bool						isDirListening;
	bool						isRedirect;
	size_t						maxBodySize;
	string						serverName;
	bool						isCgi;
	string						cgiError;
	string						timeOut;
	string						pathInfo;
	string						fileLocation;
	string						executable_file;
	std::vector<string>			redirection;
	string						default_page;
	bool						isUpload;
	std::map<int, string>		codeStatusMap;
	string						hostName;
	bool						errorFlag;
}	t_Request;

typedef struct s_Header
{
	string 						port;
	string						extension;
	string						requestMethod;
	std::vector<string>			queryStringVec;
	std::map<string, string>	bigMap;
	string						url;
	string						urlFinal;
	bool						isAlive;
}	t_Header;

typedef struct s_Body
{
	type									bodyType;
	size_t									bodySize;
	string									boundry;
	string									endBoundry;
	string									newStr;
	string									buffer;	
	string									fileName;
	string									pathFormData;
}	t_Body;


class Request 
{
	private:
		t_Request		RequestData;
		t_Header		HeaderData;
		t_Body			BodyData;
		File_Parsing	configFileObj;
		std::ofstream		outFile;
	public:
		Request();
		~Request();
		Request(const Request &src);
		Request& operator=(const Request &copy);
	
		//---------------------------------------REQUEST---------------------------------------

		File_Parsing&	getConfigFileObj();
		t_Request&		getRequestData();
		t_Header&		getHeaderData();
		t_Body&			getBodyData();
		void			request(string &body);
		void			clean(int code, string message);
		void			clearData();
		
		//---------------------------------------HEADER---------------------------------------

		void		parseFirstLine(const string &line);
		void		getTypes(const std::map<string, string> &mp);
		void		parseHeader(string &header);
		void		parseUri(string &str);
		void		storeQueryString(string &str, const size_t &QMPos);
		void		fillData(string &key, const string &value);
		void		fillHeaderMap(string &header);
		void		parseConfigInHeader();

		//---------------------------------------BODY---------------------------------------

		void		openFile(const string &fileName);
		bool		isBoundary(string &body);
		void		writeFile(string &body, int start, size_t end, size_t len);
		void		getQweryString(string &body);
		void		parseBoundryBody(string &body);
		void		parseChunkedBody(string &body);
		void		parseBodyLength(string &body);
		void		parseChunkedBoundryBody(string &body);
		int			getFileName(string &body);
		void		parseBodyTypes(string &body);
		bool		hasOneMatch(const std::string& str1, const std::string& str2);

};

string _to_string(int number);
string getNameFromTime();