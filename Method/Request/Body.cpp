/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:32 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/26 17:47:30 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::parseBodyTypes(string &body)
{
	BodyData.newStr.append(body.c_str(), body.length());
	string last = BodyData.newStr.substr(std::max(0, (int)(BodyData.newStr.length() - BodyData.endBoundry.length())));

	static u_long totalBodySize;
	totalBodySize += body.length();

	// cout << BLUE << BodyData.bodyType << RESET << endl;

	if (BodyData.bodyType == CHUNKED && body == "0\r\n\r\n")
		BodyData.bodyType = NONE;

	if (totalBodySize > RequestData.maxBodySize)
	{
		RequestData.isCgi = false;
		BodyData.outFile.close();
		string name = RequestData.fileLocation + "/" + BodyData.fileName;
		std::remove(name.c_str());
		clean(413, "Content Too Large");
		// RequestData.codeStatus = 400;
		// RequestData.requestStat = 2;
		// throw runtime_error("MAX bady szie1");
	}


	if (RequestData.isCgi &&  (BodyData.bodyType == BODY_SIZE || BodyData.bodyType == CHUNKED))
		RequestData.isCgi = false;
	
	if (hasOneMatch(last, BodyData.endBoundry) && BodyData.newStr.find(BodyData.endBoundry) == NP)
		return;
	switch (BodyData.bodyType)
	{
		case (BOUNDARY):parseBoundryBody(BodyData.newStr);	break;
		case (CHUNKED): parseChunkedBody(BodyData.newStr);	break;
		case (CHUNKED_BOUNDARY): parseChunkedBoundryBody(BodyData.newStr);	break;
		case (BODY_SIZE): parseBodyLength(BodyData.newStr);	break;
		case(NONE):	RequestData.requestStat = 2;	break;
	}
}
// 150 0 000 000

bool Request::hasOneMatch(const std::string& str1, const std::string& str2)
{
	for (size_t i = 0; i < str2.length(); ++i)
	{
		if (str1.find(str2[i]) != NP)
			return true;
	}
	return false;
}
