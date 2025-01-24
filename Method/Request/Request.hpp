/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:55 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/24 13:36:19 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "sstream"

#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
using namespace std;
#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define FILE_NAME "; filename=\""
#define NP std::string::npos


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
	string						timeOut;
	string						pathInfo;
	string						extension;
	string						fileLocation;
	string						executable_file;
	std::vector<string>			redirection;
	string						default_page;
	bool						isUpload;
}	t_Request;

typedef struct s_Header
{
	string 						port;
	string						extension;
	string						requestMethod;
	std::map<string, string>	queryStringMap;
	std::map<string, string>	bigMap;
	string						url;
}	t_Header;

typedef struct s_Body
{
	type									bodyType;
	size_t									bodySize;
	string									boundry;
	string									endBoundry;
	std::vector<std::pair<string, string> >	Vec;
	ofstream								outFile;
	string									newStr;
	string									buffer;	
	string									fileName;
}	t_Body;

#include "../../Config/File_Parsing.hpp"

class Request 
{
	private:
		t_Request		RequestData;
		t_Header		HeaderData;
		t_Body			BodyData;
	public:
		File_Parsing			configFileObj;

		File_Parsing& geto()
		{
			return configFileObj;
		}

		

	Request(const Request &src)
	{
		*this = src;
	}


	Request& operator=(const Request &copy)
	{
		if (this != &copy)
		{
			BodyData.bodyType = copy.BodyData.bodyType;
			BodyData.bodySize = copy.BodyData.bodySize;
			BodyData.boundry = copy.BodyData.boundry;
			BodyData.endBoundry = copy.BodyData.endBoundry;
			BodyData.Vec = copy.BodyData.Vec;
			BodyData.newStr = copy.BodyData.newStr;
			BodyData.buffer = copy.BodyData.buffer;
			// BodyData.outFile = copy.BodyData.outFile;
			RequestData = copy.RequestData;
			HeaderData = copy.HeaderData;
		}
		return *this;
	}
				

		Request();
		~Request();
		
		//---------------------------------------REQUEST---------------------------------------

		void		request(string &body);
		void		print(map<string, string> &headerMap);
		void		printV(vector<pair<string, string> > &mp);
		t_Request&	getRequestData();
		t_Header&	getHeaderData();
		t_Body&		getBodyData();

		//---------------------------------------HEADER---------------------------------------

		void		parseFirstLine(const string &line);
		void		getTypes(const std::map<string, string> &mp);
		void		parseHeader(string &header);
		void		parseUri(string &str);
		void		storeQueryString(string &str, const size_t &QMPos);
		void		fillData(const string &key, const string &value);

		void		fillHeaderMap(string &header);

		void		achref();

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
		bool		hasOneMatch(const std::string& str1, const std::string& str2); // change names here

};