/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:55 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/12 18:55:12 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define FILE_NAME "; filename=\""

using namespace std;

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
	
}	t_Request;

typedef struct s_Header
{
	string						extention;
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
	ofstream								*outFile;
	string									newStr;
	string									buffer;	
}	t_Body;

class Request 
{	
	public:
		t_Request		RequestData;
		t_Header		HeaderData;
		t_Body			BodyData;
		Request();
		~Request();
		
		void	request(string &body);
		void	print(map<string, string> &headerMap);
		void	printV(vector<pair<string, string> > &mp);
		string	getElement(const string & element);

		//--------------------------------------------------------------

		void	parseFirstLine(string &line);
		void	fillData(const std::map<string, string> &mp);
		const	string getExtention(std::map<string, string> mp);
		void	parseHeader(string &header);
		void	parseUri(string &str);
		void	storeQueryString(string &str, size_t QMPos);

		//-----------------------------------------------------------------------------

		void	openFile(const string &fileName);
		bool	isBoundary(string &body);
		void	writeFile(string &body, int start, size_t end, size_t len);
		void	getQweryString(string &body);
		void	parseBoundryBody(string &body);
		void	parseChunkedBody(string &body);
		void	parseBodyLength(string &body);
		void	parseChunkedBoundryBody(string &body);
		int		getFileName(string &body, string &fileName);
		void	parseBodyTypes(string &body);
		bool	hasOneMatch(const std::string& str1, const std::string& str2); // change names here
};