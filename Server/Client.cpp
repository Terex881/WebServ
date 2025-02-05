/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:42:50 by sdemnati          #+#    #+#             */
/*   Updated: 2025/02/05 13:42:56 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


Client::Client()    {}

Client::Client(const Client &src)
{
	*this = src;
}

Client& Client::operator=(const Client &copy)
{
	if (this != &copy)
	{
		req_obj = copy.req_obj;
		res_obj = copy.res_obj;
		cgi_obj = copy.cgi_obj;
	}
	return *this;
}

Response&	Client::getRes()
{
	return res_obj;
}

Request&	Client::getReq()
{
	return req_obj;
}

Cgi&	Client::getCgi()
{
	return cgi_obj;
}
