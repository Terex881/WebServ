/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:27 by sdemnati          #+#    #+#             */
/*   Updated: 2024/12/31 16:00:06 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

class Header :  public Request
{
	private:
		std::map<string, string>	bigMap;
		Request						*ataty;
		string						queryStrings;

	public:
		void	parseFirstLine(string line);
		void	fillData(const std::map<string, string> &mp);
		const	string getExtention(std::map<string, string> mp);
		void	parseHeader(string &header);
		void	parseUrl(string &str);
		
		void	setAttay(Request *reqPtr);
};
