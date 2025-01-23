/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkedBoundary.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:30 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/23 17:57:08 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::parseChunkedBoundryBody(string &body)
{
	RequestData.isUpload = true;
	size_t hexPos, strPos;
	string subBody, str;
	static u_long	length = 0;
	while(!body.empty())
	{
		if (length == 0)		
		{	
			strPos = body.find_first_of("0123456789abcdefABCDEF");	
			/* check if not 0 to avoid reallocat for nothing */
			if (strPos != 0)
			{
				/* get string before hexadecimal and write it to file and erase it */
				str = body.substr(0, strPos);
				body.erase(0, str.length());;
				if (str != CRLF && str.find_first_of(CRLF) == NP)
					parseBoundryBody(str);
			}
			/* */
			hexPos = body.find_first_not_of("0123456789abcdefABCDEF");	
			if (hexPos == NP)
			{
				cout << RED << "error: no length founded\n" << RESET;
				return;
			}
			/* get decimal number and erase hex + crlf and if length 0 erase 0 and rnrnr of crlf*/
			length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
			body.erase(0, hexPos + 2);
			if (!length)
			{
				body.erase(0, 4);	return;
			}
		}
		subBody = body.substr(0, length);
		BodyData.buffer.append(subBody.c_str(), subBody.length());
		length -= subBody.length();

		body.erase(0, subBody.length());
		parseBoundryBody(BodyData.buffer);
	}
}