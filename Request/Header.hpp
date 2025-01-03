/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:27 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/01 17:45:26 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

class Header :  public Request
{
	private:
		std::map<string, string>	bigMap;
		string						queryStrings;
		Request						*_headerPtr;

		void	parseFirstLine(string &line);
		void	fillData(const std::map<string, string> &mp);
		const	string getExtention(std::map<string, string> mp);
		void	parseUri(string &str);
	public:
		void	parseHeader(string &header);
		void	setAttay(Request *reqPtr);
		const	std::map<string, string>	getMap() const;
		Header()
		{
			_headerPtr = NULL;
			_headerPtr = new Header();

		}
};
