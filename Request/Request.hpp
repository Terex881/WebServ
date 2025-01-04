/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:55 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/04 16:38:50 by sdemnati         ###   ########.fr       */
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
	string					boundry;
	string					endBoundry;
	string					extention;
	string					header;
	size_t					bodySize;
	type					bodyType;
	int						requestStat;
	Body*					body_obj;
	Header*					header_obj;
	string					requestMethod;
}	t_Data;


class Request
{
	protected:
	
	public:
		std::map<int, Request> clientsMap;
		t_Data requestData;
		Request();
		~Request();

		void	request(string &body);
		
		// t_Data	getStat() const
		// {
		// 	return d;
		// }
		// void	setStat(t_Data	_stat)
		// {
		// 	d = _stat;
		// }


		void	print(map<string, string> &headerMap);
		void	printV(vector<pair<string, string> > &mp);

		Header*	getHeader();
		Body*	getBody();
		string	getElement(const string & element);
};