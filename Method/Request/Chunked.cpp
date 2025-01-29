/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:40 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/29 13:55:43 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

string _to_string(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

string getNameFromTime()
{
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);
	return (_to_string(localTime->tm_hour) + ":" + _to_string(localTime->tm_min) + ":" + _to_string(localTime->tm_sec));
}

void Request::parseChunkedBody(string &body)
{
	RequestData.isUpload = true;
	openFile(RequestData.fileLocation + "/" + getNameFromTime());

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
				body.erase(0, str.length());
				if (str != CRLF && str.find_first_of(CRLF) == NP)
					writeFile(str, 0, str.length(), 0);
			}
			hexPos = body.find_first_not_of("0123456789abcdefABCDEF");	
			if (hexPos == NP)
			{
				cout << RED << "error: no length founded\n" << RESET;
				return;
			}
			
			length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
			body.erase(0, hexPos + 2);

			if (!length)
			{
				/* erase last DCRLF */
				body.erase(0, 4);
				/* if strtol fails will return 0 */
				if (!body.empty())
				{
					cout << RED << "error: strtol fails\n" << RESET;
				}
				BodyData.outFile.close();
				RequestData.requestStat = 2;
				RequestData.codeStatus = 201;
				return;
			}
		}
		subBody = body.substr(0, length);
		length -= subBody.length();
		writeFile(body, 0, subBody.length(), 0);
	}
}


void Request::parseBodyLength(string &body)
{
	RequestData.isUpload = true;
	openFile(RequestData.fileLocation + "/" + getNameFromTime());
	
	size_t tmp = BodyData.bodySize;
	tmp -= body.length();
	BodyData.bodySize = tmp;
	writeFile(body, 0, body.length(), 0);
	if (!BodyData.bodySize)
	{
		RequestData.requestStat = 2;
		RequestData.codeStatus = 201;
		BodyData.outFile.close();
	}
}