/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:37 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/01 17:45:22 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Body.hpp"

void	Body::writeFile(string &body, int start, size_t end, size_t len)
{
	string content = body.substr(start, end);
	if (outFile.is_open())
		outFile.write(content.c_str(), content.length()); outFile.flush();
	body.erase(0, end + len);
}

void	Body::openFile(string fileName)
{
	if (!outFile.is_open())
	{
		outFile.open(fileName, ios::binary | ios::app);
		if (!outFile.is_open())
			cout << RED << "FAILED OPEN" + fileName << endl, exit(1); // fix
	}
}

void	Body::getQweryString(string &body)
{
	string key, val;
	size_t contentEndtPos = 0, namePos = body.find("; name=\"");
	size_t crlfPos, endBoundyPos;

	body.erase(0, namePos + 8);
	key = body.substr(0, body.find(CRLF) - 1);
	body.erase(0, body.find(DCRLF) + 4);
	
	if ((crlfPos = body.find(CRLF)) != std::string::npos)
		contentEndtPos = crlfPos;
	else if ((endBoundyPos = body.find(_bodyPtr->getEndB())) != std::string::npos)
		contentEndtPos = endBoundyPos;
	else
		contentEndtPos = body.length();
	
	val = body.substr(0, contentEndtPos);
	Vec.push_back(make_pair(key, val));
	body.erase(0, val.length());
}

int	Body::getFileName(string &body, string &fileName)
{
	string tmp = body.substr(body.find(_bodyPtr->getB()) + _bodyPtr->getB().length(), body.length());
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
		if (tmp.find(_bodyPtr->getB()) == string::npos && tmp.find(_bodyPtr->getEndB()) == string::npos)
			return 0;
		getQweryString(body);
	}
	return -1;
}

bool	Body::isBoundary(string &body)
{
	size_t	contentEndtPos = 0, endboundryPos = 0, boundryPos = body.find(_bodyPtr->getB());
	string	fileName;

	if (boundryPos != 0) writeFile(body, 0, boundryPos - 2, 2);
	else writeFile(body, 0, boundryPos, 0);
	outFile.close();
	
	int i = getFileName(body, fileName);
	if (i == 1)
	{
		body.erase(0, body.find(DCRLF) + 4);
		boundryPos = body.find(_bodyPtr->getB());
		endboundryPos = body.find(_bodyPtr->getEndB());

		if (boundryPos != std::string::npos)
			contentEndtPos = boundryPos;
		else if ( endboundryPos != std::string::npos )
			contentEndtPos = endboundryPos;
		else
			contentEndtPos = body.length();

		openFile("Zip/" + fileName);
		writeFile(body, 0, contentEndtPos, 0);
	}
	else if (!i)
		return 0;
	return 1;
}

void	Body::parseBoundryBody(string &body)
{
	ofstream ss("Z.py", ios::app);
	ss << body;
	ss << "\n--------------------------------------------\n";

	size_t boundryPos, endboundryPos;
	endboundryPos = body.find(_bodyPtr->getEndB());
	while(!body.empty())
	{
		boundryPos = body.find(_bodyPtr->getB());
		if (boundryPos == string::npos && endboundryPos == string::npos)
		{
			if (body == CRLF && _bodyPtr->getType() == 2) body.erase(0, 2);
			else writeFile(body, 0, body.length(), 0);
		}
		if (boundryPos != string::npos)
		{
			if (!isBoundary(body)) return;
		}
		else if (endboundryPos != string::npos)
		{
			endboundryPos = body.find(_bodyPtr->getEndB());	
			writeFile(body, 0, endboundryPos, _bodyPtr->getEndB().length());
			printV(Vec);
			_bodyPtr->setStat(2);
		}
	}
}

