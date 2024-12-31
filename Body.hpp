/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:35 by sdemnati          #+#    #+#             */
/*   Updated: 2024/12/31 16:45:36 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

class Body :  public Request
{
	private:
		ofstream outFile;
		std::vector<std::pair<string, string> > Vec;
		string newStr;
		string buffer;


		void openFile(string fileName);
		bool isBoundary(string &body);
		void writeFile(string &body, int start, size_t end, size_t len);
		void getQweryString(string &body);
		void parseBoundryBody(string &body);
		void parseChunkedBody(string &body);
		void parseBodyLength(string &body);
		void parseChunkedBoundryBody(string &body);
		int getFileName(string &body, string &fileName);

		Request *atay_tkhwa;


	public:
		void parseBodyTypes(string &body);
		void	setAttay(Request *reqPtr);
};
