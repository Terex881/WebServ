/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:32 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/09 13:29:11 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"
#include "Request.hpp"

void	Body::parseBodyTypes(string &body)
{
	newStr.append(body.c_str(), body.length());
	string last = newStr.substr(std::max(0, (int)(newStr.length() - atay_tkhwa->clientData.endBoundry.length())));

	if (hasOneMatch(last, atay_tkhwa->clientData.endBoundry) && newStr.find(atay_tkhwa->clientData.endBoundry) == string::npos)
		return;

	switch (atay_tkhwa->clientData.bodyType)
	{
		case (BOUNDARY):parseBoundryBody(newStr); break;
		case (CHUNKED): parseChunkedBody(newStr); break;
		case (CHUNKED_BOUNDARY): parseChunkedBoundryBody(newStr); break;
		case (BODY_SIZE): parseBodyLength(newStr); break;
		case(NONE): break;
	}
	
}

void	Body::setAttay(Request *reqPtr)
{
	atay_tkhwa = reqPtr;
}

bool Body::hasOneMatch(const std::string& str1, const std::string& str2)
{
	for (size_t i = 0; i < str2.length(); ++i)
	{
		if (str1.find(str2[i]) != string::npos)
			return true;
	}
	return false;
}
