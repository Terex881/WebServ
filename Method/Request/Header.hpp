/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:27 by sdemnati          #+#    #+#             */
/*   Updated: 2025/01/09 12:07:07 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

class Header :  public Request
{
	private:
		Request						*ataty;

	public:
		std::map<string, string>	bigMap;

		
		void	parseFirstLine(string &line);
		void	fillData(const std::map<string, string> &mp);
		const	string getExtention(std::map<string, string> mp);
		void	parseHeader(string &header);
		void	parseUri(string &str);

		void	storeQueryString(string &str, size_t QMPos);
		
		void	setAttay(Request *reqPtr);



const std::map<string, string> &getMap() const
{
	return bigMap;	
}
};
