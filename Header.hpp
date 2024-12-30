#pragma once

#include "Request.hpp"

class Header : public Request
{
	private:
		std::map<string, string>	bigMap;

	public:
		// void			parseFirstLine(string &line, map<string, string> &mp);
		void			parseFirstLine(string line);
		void			fillData(const std::map<string, string> &mp, Request *req); // for fill data
		const string	getExtention(std::map<string, string> mp);
		void			parseHeader(string &header, Request* req);

};