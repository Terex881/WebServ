/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:56:21 by sdemnati          #+#    #+#             */
/*   Updated: 2025/02/06 12:17:38 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once 

#include "../Response.hpp"
#include "../Method/Request/Request.hpp"
#include "../cgi-bin/Cgi.hpp"

class Client
{
	private:
		Request		req_obj;
		Response	res_obj;
		Cgi			cgi_obj;
	public:
		Client();
		~Client();
		// Client(const Client &src);
		// Client& operator=(const Client &copy);

		struct kevent *event;
		Response&	getRes();
		Request&	getReq();
		Cgi&	getCgi();
};