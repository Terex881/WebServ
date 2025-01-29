/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:56:21 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/29 17:51:06 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once 

#include "Response.hpp"
#include "Method/Request/Request.hpp"
#include "./cgi-bin/Cgi.hpp"

class Client
{
	public:
		Request		req_obj;
		Response	res_obj;
		Cgi			cgi_obj;
		struct kevent *event;

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

	Client()
	{
		// this->event = event;
	}


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
			// this->event = copy.event;
		}
		return *this;
	}
};