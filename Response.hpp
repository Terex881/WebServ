	#ifndef RESPONSE_HPP
#define RESPONSE_HPP


#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <dirent.h>
#include "Method/Delete.hpp"
#include "./Config/DynamicStruct.hpp"

using namespace std;

using std::string;
using std::vector;
using std::map;
#define MAX_CLIENTS 128

class Response : public Delete
{
	public :
		size_t			Status_Code;
		int ss;
		string			Content_Type;
		string			Reason_Phrase;
		size_t			Content_Length;
		string			Working_Path;
		string			Method;
		std::streamsize current_read;
		std::ifstream	file;
		string			Url;
		string			first;
		size_t			Res_Size;
		int				end;
		const size_t	Chunk_Size;
		int				idx;
		std::streamsize	bytesRead;
		char			buffer[8192];
		std::stringstream	responseStream;
		string			header;
		string			body;
		bool			isLesn;
		string			filename;
		vector<string> redirection;
		string			default_page;
		bool			isUpload;
		static int		cookie_gen;
		int				client_cookie;
		bool			isCgi;
		DynamicStruct	server;
		int				unlink_cgi;
		string			urlFinal;
		string			timeOut;
		string			cgiError;
		map<int, string> codeStatusMap;
		string			tmp_Status_Code;
	
		Response();
		Response(string content_type, string working_path, string method,
					string Url, int codeStatus, bool isLesn, string filename,
					vector<string> redirection, string default_page, bool isUpload,
					bool isCgi, DynamicStruct server, string urlFinal, string timeOut,
					string cgiError, map<int, string> codeStatusMap);

		Response(const Response& other);
		Response&operator=(const Response& other);

		bool	isFile(const std::string& path);
		bool	isDirectory(const std::string& path);
		void	Res_get_chunk(int &sent_head);
		static string	GetMimeType(const std::string& filename);
		void	handle_cgi_response(int &sent_head);
		static size_t	Calculate_File_Size(std::ifstream &file);
		int		res_delete(int &sent_head);
		int		res_post(int &sent_head);
};

#endif
