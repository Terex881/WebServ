/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:55 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/11 20:51:16 by sdemnati         ###   ########.fr       */
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
class Header;
class Body;

enum type
{
	BOUNDARY,
	CHUNKED,
	CHUNKED_BOUNDARY,
	BODY_SIZE,
	NONE,
};

typedef struct s_Data
{
	std::map<string, string>	queryStringMap;
	string						boundry;
	string						endBoundry;
	string						extention;
	string						header;
	size_t						bodySize;
	type						bodyType;
	int							requestStat;
	string						requestMethod;
	//------------------------------------------------------
	int fd;
	// struct sockaddr_in addr;
	int	is_server;
	int	is_client;
	int sent_head;
	string	url;
	string	first;
	// Response rsp;
	size_t	bytes_sent;
	std::ifstream *file;
	
}	t_Data;

class Client;
// #include "../../Client.hpp"

class Request 
{	
	public:
	Body*						body_obj;
	Header*						header_obj;
	t_Data clientData;

	// Client						client_obj;

	

	
		Request();
		~Request();

		void	request(string &body);

		void	print(map<string, string> &headerMap);
		void	printV(vector<pair<string, string> > &mp);

		Header*	getHeader();
		Body*	getBody();
		string	getElement(const string & element);

		// Client& get()
		// {
		// 	return client_obj;
		// }
};