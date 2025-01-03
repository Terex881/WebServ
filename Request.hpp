/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:55 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/03 18:47:09 by sdemnati         ###   ########.fr       */
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

typedef struct s_Data
{
	string					boundry;
	string					endBoundry;
	string					extention;
	string					header;
	size_t					bodySize;
	int						TYPE;
	int						REQUEST_IS_FINISH;

	Body*					body_obj;
	Header*					header_obj;

	
} t_Data;


class Request
{
	protected:
	
	public:
		t_Data d;
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

		Body*	getBody();
		Header*	getHeader();
};