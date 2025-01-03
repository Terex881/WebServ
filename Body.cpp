/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:32 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/03 18:45:47 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"

void	Body::parseBodyTypes(string &body)
{
	newStr.append(body.c_str(), body.length());
	string last = newStr.substr(std::max(0, (int)(newStr.length() - atay_tkhwa->d.endBoundry.length())));
	if (hasOneMatch(last, atay_tkhwa->d.endBoundry) && newStr.find(atay_tkhwa->d.endBoundry) == string::npos)
		return;
			
	switch (atay_tkhwa->d.TYPE)
	{
		case (0):parseBoundryBody(newStr); break;
		case (1): parseChunkedBody(newStr); break;
		case (2): parseChunkedBoundryBody(newStr); break;
		case (3): parseBodyLength(newStr); break;
	}
}

void	Body::setAttay(Request *reqPtr)
{
	atay_tkhwa= reqPtr;
}