/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdemnati <sdemnati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:52:40 by sdemnati          #+#    #+#             */
/*   Updated: 2024/12/31 15:52:41 by sdemnati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Body.hpp"

void Body::parseChunkedBody(string &body)
{
	openFile("Zip/ok." + atay_tkhwa->getEx());

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
				body.erase(0, str.length());;
				if (str != CRLF && str.find_first_of(CRLF) == string::npos)
				{
					exit(1);
					parseBoundryBody(str);
				}
			}
			hexPos = body.find_first_not_of("0123456789abcdefABCDEF");	
			if (hexPos == string::npos)
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
				outFile.close();
				atay_tkhwa->setStat(2);
				return;
			}
		}
		subBody = body.substr(0, length);
		length -= subBody.length();
		writeFile(body, 0, subBody.length(), 0);
		// body.erase(0, subBody.length());

		/* the chunk is full and gonna read new chunk*/
		if (!length)
			body.erase(0, 2);	
	}
}

void Body::parseBodyLength(string &body)
{
	openFile("Zip/ok." + atay_tkhwa->getEx());

	// static u_long length = 0;
	// bodySize -= body.length();
	size_t tmp = atay_tkhwa->getSize();
	tmp -= body.length();
	atay_tkhwa->setSize(tmp);
	if (!atay_tkhwa->getSize())
		atay_tkhwa->setStat(2);
	cout << RED << atay_tkhwa->getSize() << endl;
	writeFile(body, 0, body.length(), 0);
}



// void Body::parseBodyLength(string &body)
// {
	
// 	openFile("Zip/ok." + extention);

// 	// static u_long length = 0;
// 	string subBody = body.substr(0, bodySize);

// 	bodySize -= subBody.length();

// 	writeFile(body, 0, body.length(), 0);
// }