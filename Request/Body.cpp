/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:32 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/01 17:45:22 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"

void	Body::parseBodyTypes(string &body)
{
	newStr.append(body.c_str(), body.length());

	switch (_bodyPtr->getType())
	{
		case (0):parseBoundryBody(newStr); break;
		case (1): parseChunkedBody(newStr); break;
		case (2): parseChunkedBoundryBody(newStr); break;
		case (3): parseBodyLength(newStr); break;
	}
}

void	Body::setAttay(Request *reqPtr)
{
	cout << RED << reqPtr << RESET << endl;
	cout << BLUE << _bodyPtr << RESET << endl;
	this->_bodyPtr= reqPtr;
}
