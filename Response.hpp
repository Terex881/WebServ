#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>

using std::string;
#define MAX_CLIENTS 128

class Response
{
	public :
		const size_t	Chunk_Size;
		size_t			Status_Code;
		string			Content_Type;
		string			Reason_Phrase;
		size_t			Content_Length;
		string			Working_Path;
		string			Method;
		string			Url;
		std::ifstream	*file;
		size_t			Res_Size;
		int				end;
		int				idx;
		static int		sent_head[MAX_CLIENTS];
		static int sre;
		std::streamsize	bytesRead;
		std::streamsize current_read;
		char			buffer[8192];
		static	std::stringstream responseSStream;

		string			header;
		string			body;
		Response(size_t status_code, size_t content_length, string content_type, string reason_phrase, string working_path, string method, std::ifstream *file, int idx, string Url);
		bool	isFile(const std::string& path);
		bool	isDirectory(const std::string& path);
		void	Res_get_chunk(std::stringstream &responseStream);
		static string	GetMimeType(const std::string& filename);
};

#endif
