/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:35 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/01 17:43:21 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

class Body :  public Request
{
	private:
		std::vector<std::pair<string, string> >	Vec;
		ofstream								outFile;
		string									newStr;
		string									buffer;
		Request									*_bodyPtr;

		void	openFile(string fileName);
		bool	isBoundary(string &body);
		void	writeFile(string &body, int start, size_t end, size_t len);
		void	getQweryString(string &body);
		void	parseBoundryBody(string &body);
		void	parseChunkedBody(string &body);
		void	parseBodyLength(string &body);
		void	parseChunkedBoundryBody(string &body);
		int		getFileName(string &body, string &fileName);

	public:
		void parseBodyTypes(string &body);
		void	setAttay(Request *reqPtr);
};
