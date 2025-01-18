/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:56:21 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/15 15:17:40 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once 

#include "Response.hpp"
#include "Method/Request/Request.hpp"
#include "./cgi-bin/Cgi.hpp"
// class Cgi;

class Client
{
	public:
		Request		req_obj;
		Response	res_obj;
		Cgi			cgi_obj;

	Response&	getRes()
	{
		return res_obj;
	}
	Request&	getReq()
	{
		return req_obj;
	}
	Cgi&	getCgi()
	{
		return cgi_obj;
	}

	Client() {}

	Client(const Client &src)
	{
		*this = src;
	}

	Client& operator=(const Client &copy)
	{
		if (this != &copy)
		{
			req_obj = copy.req_obj;
			res_obj = copy.res_obj;
			cgi_obj = copy.cgi_obj;
		}
		return *this;
	}
};