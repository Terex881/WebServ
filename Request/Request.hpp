/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:55 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/01 14:15:36 by sdemnati         ###   ########.fr       */
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

class Request
{
	protected:
		string					boundry;
		string					endBoundry;
		string					extention;
		size_t					bodySize;
		int						TYPE;
		int						REQUEST_IS_FINISH;
		string					header;

		Body*					body_obj;
		Header*					header_obj;

	public:

		Request();
		~Request();

		void	request(string &body);
		int		getStat() const;
		int		getType() const;
		string	getB() const;
		string	getEndB() const;
		string	getEx() const;
		size_t	getSize() const;
		
		void	setStat(const int &_stat);
		void	setType(const int &_stat);
		void	setB(const string &_B);
		void	setEndB(const string &_Eb);
		void	setEx(const string &_Ex) ;
		void	setSize(const size_t &_Size);
		


		string	getElement(const string & element);


		void	print(map<string, string> &headerMap);
		void	print1(const map<string, string> &headerMap);
		
		void	printV(vector<pair<string, string> > &mp);

		Body*	getBody();
		Header*	getHeader();
};