/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:37 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/15 11:21:32 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::writeFile(string &body, int start, size_t end, size_t len)
{
	string content = body.substr(start, end);
	if (BodyData.outFile.is_open())
		BodyData.outFile.write(content.c_str(), content.length()); BodyData.outFile.flush();
	body.erase(0, end + len);
}

void	Request::openFile(const string &fileName)
{
	if (!BodyData.outFile.is_open())
	{
		BodyData.outFile.open(fileName, ios::binary | ios::trunc);
		if (!BodyData.outFile.is_open())
		{
			RequestData.codeStatus = 505;
			RequestData.requestStat = 2;
			cout << RED << "FAILED OPEN" + fileName << endl;//, exit(1); // fix
		}
	}
}



void	Request::getQweryString(string &body)
{
	string key, val;
	size_t contentEndtPos = 0, namePos = body.find("; name=\"");
	size_t crlfPos, endBoundyPos;

	body.erase(0, namePos + 8);
	key = body.substr(0, body.find(CRLF) - 1);
	body.erase(0, body.find(DCRLF) + 4);
	
	if ((crlfPos = body.find(CRLF)) != std::string::npos)
		contentEndtPos = crlfPos;
	else if ((endBoundyPos = body.find(BodyData.endBoundry)) != std::string::npos)
		contentEndtPos = endBoundyPos;
	else
		contentEndtPos = body.length();
	
	val = body.substr(0, contentEndtPos);
	BodyData.Vec.push_back(make_pair(key, val));
	body.erase(0, val.length());
}

int	Request::getFileName(string &body, string &fileName)
{
	string tmp = body.substr(body.find(BodyData.boundry) + BodyData.boundry.length(), body.length());
	string first = tmp.substr(0, tmp.find(CRLF) + 2);

	if (first.find("\"\r\n") == string::npos || body.find(DCRLF) == string::npos)
	{
		cout << RED << "error: no CRLF founded\n" << RESET;
		return 0;
	}
	size_t filePos = first.find(FILE_NAME);
	if (filePos != std::string::npos)
	{
		first.erase(0, filePos + 12);
		size_t end = first.rfind("\"\r\n");
		size_t endV = first.rfind("\"; ");
		if (end != string::npos)
			fileName = first.substr(0, end);
		else if (endV != string::npos)
			fileName = first.substr(0, endV);
		return (1);
	}
	else
	{
		if (tmp.find(BodyData.boundry) == string::npos && tmp.find(BodyData.endBoundry) == string::npos)
			return 0;
		getQweryString(body);
	}
	return -1;
}

bool	Request::isBoundary(string &body)
{
	size_t	contentEndtPos = 0, endboundryPos = 0, boundryPos = body.find(BodyData.boundry);
	string	fileName;

	if (boundryPos != 0) writeFile(body, 0, boundryPos - 2, 2);
	else writeFile(body, 0, boundryPos, 0);
	BodyData.outFile.close();
	
	int i = getFileName(body, fileName);
	if (i == 1)
	{
		body.erase(0, body.find(DCRLF) + 4);
		boundryPos = body.find(BodyData.boundry);
		endboundryPos = body.find(BodyData.endBoundry);

		if (boundryPos != std::string::npos)
			contentEndtPos = boundryPos;
		else if ( endboundryPos != std::string::npos )
			contentEndtPos = endboundryPos;
		else
			contentEndtPos = body.length();

		openFile("/Users/sdemnati/goinfre/UP/" + fileName);
		writeFile(body, 0, contentEndtPos, 0);
	}
	else if (!i)
		return 0;
	return 1;
}

void	Request::parseBoundryBody(string &body)
{
	size_t boundryPos, endboundryPos;
	endboundryPos = body.find(BodyData.endBoundry);
	while(!body.empty())
	{
		boundryPos = body.find(BodyData.boundry);
		if (boundryPos == string::npos && endboundryPos == string::npos)
		{
			if (body == CRLF && BodyData.bodyType == CHUNKED_BOUNDARY) body.erase(0, 2);
			else writeFile(body, 0, body.length(), 0);
		}
		if (boundryPos != string::npos)
		{
			if (!isBoundary(body)) return;
		}
		else if (endboundryPos != string::npos)
		{
			endboundryPos = body.find(BodyData.endBoundry);	
			writeFile(body, 0, endboundryPos, BodyData.endBoundry.length());
			printV(BodyData.Vec);
			RequestData.requestStat = 2;
		}
	}
}

