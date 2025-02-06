/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:37 by sdemnati          #+#    #+#             */
/*   Updated: 2025/02/06 12:12:42 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::writeFile(string &body, int start, size_t end, size_t len)
{
	string content = body.substr(start, end);
	if (outFile.is_open())
	{
		outFile.write(content.c_str(), content.length()); outFile.flush();
		if (outFile.bad())
			clean(500, "Internal Server Error");
	}
	body.erase(0, end + len);
}

void	Request::openFile(const string &name)
{
	if (!outFile.is_open())
	{
		outFile.open(name, ios::binary | ios::trunc);
		if (!outFile.is_open() && RequestData.errorFlag)
			clean(500, "Internal Server Error");
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
	
	if ((crlfPos = body.find(CRLF)) != NP)
		contentEndtPos = crlfPos;
	else if ((endBoundyPos = body.find(BodyData.endBoundry)) != NP)
		contentEndtPos = endBoundyPos;
	else
		contentEndtPos = body.length();
	
	val = key + "=" + body.substr(0, contentEndtPos);
	HeaderData.queryStringVec.push_back(val);
	body.erase(0, val.length() - key.length() - 1);
}

int	Request::getFileName(string &body)
{
	string tmp = body.substr(body.find(BodyData.boundry) + BodyData.boundry.length(), body.length());
	string first = tmp.substr(0, tmp.find(CRLF) + 2);

	if (first.find("\"\r\n") == NP || body.find(DCRLF) == NP)
		return 0;
	size_t filePos = first.find(FILE_NAME);
	if (filePos != NP)
	{
		first.erase(0, filePos + 12);
		size_t end = first.rfind("\"\r\n");
		size_t endV = first.rfind("\"; ");
		if (end != NP)
			BodyData.fileName = first.substr(0, end);
		else if (endV != NP)
			BodyData.fileName = first.substr(0, endV);
		return (1);
	}
	else
	{
		if (tmp.find(BodyData.boundry) == NP && tmp.find(BodyData.endBoundry) == NP)
			return 0;
		getQweryString(body);
	}
	return -1;
}

bool	Request::isBoundary(string &body)
{
	size_t	contentEndtPos = 0, endboundryPos = 0, boundryPos = body.find(BodyData.boundry);

	if (boundryPos != 0) writeFile(body, 0, boundryPos - 2, 2);
	else writeFile(body, 0, boundryPos, 0);
	outFile.close();
	
	int i = getFileName(body);
	if (i == 1)
	{
		body.erase(0, body.find(DCRLF) + 4);
		boundryPos = body.find(BodyData.boundry);
		endboundryPos = body.find(BodyData.endBoundry);

		if (boundryPos !=  NP)
			contentEndtPos = boundryPos;
		else if ( endboundryPos !=  NP )
			contentEndtPos = endboundryPos;
		else
			contentEndtPos = body.length();
		BodyData.pathFormData = RequestData.fileLocation + "/" + BodyData.fileName;
		openFile(BodyData.pathFormData);
		writeFile(body, 0, contentEndtPos, 0);
	}
	else if (!i)
		return 0;
	return 1;
}

void	Request::parseBoundryBody(string &body)
{
	RequestData.isUpload = true;
	size_t boundryPos, endboundryPos;
	endboundryPos = body.find(BodyData.endBoundry);
	while(!body.empty())
	{
		boundryPos = body.find(BodyData.boundry);
		if (boundryPos == NP && endboundryPos == NP)
		{
			if (body == CRLF && BodyData.bodyType == CHUNKED_BOUNDARY) body.erase(0, 2);
			else writeFile(body, 0, body.length(), 0);
		}
		if (boundryPos != NP)
		{
			if (!isBoundary(body)) return;
		}
		else if (endboundryPos != NP)
		{
			endboundryPos = body.find(BodyData.endBoundry);	
			writeFile(body, 0, endboundryPos, BodyData.endBoundry.length());
			// printV(BodyData.Vec);
			RequestData.requestStat = 2;
			RequestData.codeStatus = 201;
			outFile.close();
		}
	}
}

