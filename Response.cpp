#include "./Response.hpp"
#include "Method/Request/Request.hpp"
#include "./Method/Delete.hpp"
#include <iostream>
#include <string>

int Response::cookie_gen = 0;

size_t	Response::Calculate_File_Size(std::ifstream &file)
{
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	return fileSize;
}

Response::Response():Chunk_Size(1024)
{

}

Response::Response(string content_type,\
					string working_path, string method, string Url, int codeStatus, bool isLesn, string filename, vector<string> redirection, string default_page, bool	isUpload, bool isCgi, DynamicStruct server, string urlFinal, string timeOut, string cgiError, map<int, string> codeStatusMap):Delete("", Url, 1, isCgi),Chunk_Size(1024) // 8 KB chunks 8192
{
	this->Content_Type = content_type;
	this->Working_Path = working_path;
	this->Method =	method;
	this->current_read = 0;
	this->Url = Url;
	this->end = 0;
	this->Status_Code = (size_t)codeStatus;
	this->isLesn = isLesn;
	this->redirection = redirection;
	this->default_page = default_page;
	this->isUpload = isUpload;
	this->isCgi = isCgi;
	this->server = server;
	this->unlink_cgi = 0;
	this->urlFinal = urlFinal;
	this->timeOut = timeOut;
	this->cgiError = cgiError;
	this->codeStatusMap = codeStatusMap;
	if (!filename.empty())
		this->filename = filename;
}

Response::Response(const Response& other)
		: Content_Type(other.Content_Type),
			Working_Path(other.Working_Path),
			Method(other.Method),
			current_read(other.current_read),
			Url(other.Url),
			end(other.end),
			Chunk_Size(other.Chunk_Size),
			filename(other.filename),
			redirection(other.redirection),
			default_page(other.default_page),
			isUpload(other.isUpload),
			isCgi(other.isCgi)
			{
		if (!filename.empty())
			file.open(filename, std::ios::binary);
}

Response& Response::operator=(const Response& other)
{
	if (this != &other)
	{
		Content_Type = other.Content_Type;
		Working_Path = other.Working_Path;
		Method = other.Method;
		current_read = other.current_read;
		Url = other.Url;
		end = other.end;

		Status_Code = other.Status_Code;
		isLesn = other.isLesn;
		redirection = other.redirection;
		default_page = other.default_page;
		isUpload = other.isUpload;
		isCgi = other.isCgi;
		server = other.server;
		unlink_cgi = other.unlink_cgi;
		urlFinal = other.urlFinal;
		timeOut = other.timeOut;
		cgiError = other.cgiError;
		codeStatusMap = other.codeStatusMap;
		if (file.is_open())
			file.close();

		// Open new file
		if (!other.filename.empty())
		{
			filename = other.filename;
			file.open(filename, std::ios::binary);
		}
	}
    return *this;
}

string	ret_header(int code_status, string status_msg, string res_msg, string type)
{
	string header;

	header = "HTTP/1.1 "+ _to_string(code_status) +" "+status_msg +"\r\n"
	"Content-Type: " + type + "\r\n"
	"Content-Length: "+ _to_string(res_msg.length()) +"\r\n"
	"\r\n"+
	res_msg;

	return header;
}

void	Response::handle_cgi_response(int &sent_head)
{
	char buffer[4096];
	std::string content;
	std::string default_hd;
	size_t set = std::string::npos;
	size_t offset = std::string::npos;
	this->bytesRead = 0;
	sent_head = 1;
	isCgi = false;
	
	cout << YELLOW << ";; " << Working_Path << RESET << endl;
	Res_Size = Calculate_File_Size(file);
	int log = 0;
	cout << "ddd ................... " << server.values[_to_string(Status_Code)] << endl;
	if (!server.values[_to_string(Status_Code)].empty())
	{
		tmp_Status_Code = "3333";
		return;
	}
	cout << "ENTER ................... " << endl;
	// invalid syntax CGI
	if (!cgiError.empty())
	{
		tmp_Status_Code = "200";

		header = "HTTP/1.1 500 Internal Server Error\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: "+ _to_string(Calculate_File_Size(file) + 3) +"\r\n"
				"\r\n";
		responseStream.write(header.c_str(), header.length());
		return;
	}
	default_hd = "HTTP/1.1 200 OK\r\n"
				"Transfer-Encoding: chunked\r\n"
				"Connection: keep-alive\r\n"
				"Content-Type: text/html\r\n\r\n";

	if (!timeOut.empty())
		return;
	while (file.read(buffer, 4096) || file.gcount() > 0)
	{
		size_t bytesRead = file.gcount();

		// Append the read data to content
		content.append(buffer, bytesRead);
		if (content.find("Set-Cookie:") != string::npos)
			log = 1;
		offset = content.find("Content-Type: text/html\r\n\r\n");
		if (offset != string::npos)
			break ;
	}

	if (content != default_hd && !log)
	{
		if (offset == std::string::npos)
			set = 0;
		else
			set = offset + 27;
		content = default_hd;
	}
	else if (offset == std::string::npos && log)
	{
		header = 
			"HTTP/1.1 500 Internal Error\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 15\r\n"
			"\r\n"
			"Internal Errorr";
		responseStream.write(header.c_str(), header.length());
		this->end = 1;
		return;
	}
	if (log && offset != string::npos)
	{
		header = content.substr(0, offset + 27);
	}
	else if (!set)
		header = default_hd;
	else
		header = content.substr(0, set);

	responseStream.write(header.c_str(), header.length());
	file.clear();
	if (set != std::string::npos)
		file.seekg(set, std::ios::beg);
	else
		file.seekg(offset + 27, std::ios::beg);
}


void	Response::Res_get_chunk(int &sent_head)
{
	std::vector<char> buffer(Chunk_Size, 0);
	string default_idx;
	responseStream.str(""); // Clear previous content
	responseStream.clear(); // Clear any error flags

	
	if (tmp_Status_Code.empty())
		tmp_Status_Code = _to_string(Status_Code);
	// cout << BLUE << "Method : " << Method << " | Status_Code : " << Status_Code  << "  |  Url : " << Url << "  || isCgi : " << isCgi<< RESET << endl;
	// cout << GREEN << "Working_Path : " << Working_Path << RESET<< endl;

	if (Method != "GET" && Method != "POST" && Method != "DELETE")
	{
		header = 
			"HTTP/1.1 " + _to_string(Status_Code) +" Internal Error\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 11\r\n"
			"\r\n"
			"Bad Request";
		responseStream.write(header.c_str(), header.length());
		this->end = 1;
		return;
	}
	if (redirection.size())
	{
		header = 
		"HTTP/1.1 "+ redirection[0] +" Found\r\n"
		"Location: " + redirection[1] + "\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 0\r\n"
		"Connection: close\r\n\r\n";
		responseStream.write(header.c_str(), header.length());
		this->end = 1;
		return;
	}
	if (Method == "DELETE")
	{
		// res error msg
		if(!codeStatusMap[Status_Code].empty())
		{
			// defaul err page
			default_idx = server.values[_to_string(Status_Code)];
			if (!default_idx.empty() && isFile(default_idx))
			{
				Working_Path = default_idx;
				file.close();
				sent_head = 0;
				filename = default_idx;
				tmp_Status_Code = "200";
				Content_Type = GetMimeType(default_idx);
				file.open(default_idx, std::ios::binary);
				Method = "GET";
				return;
			}
			else
				header = "HTTP/1.1 " + _to_string(Status_Code) + " Bad Request\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: " + _to_string(codeStatusMap[Status_Code].length()) + "\r\n"
					"\r\n"+	codeStatusMap[Status_Code];
			responseStream.write(header.c_str(), header.length());
			this->end = 1;	return ;
		}
		else if (Status_Code == 405)
			delete_config = 0;
		else
			delete_config = 1;
		Delete a("",Working_Path,1, isCgi);
		a.Delete_File();
		responseStream.write(a.response.c_str(), a.response.length());
		this->end = 1;	return ;
	}
	if (isCgi)
	{
		handle_cgi_response(sent_head);
		Method = "GET";
		unlink_cgi = 1;
		return;
	}
	else if ((Method == "POST"))
	{
		if (!isFile(Working_Path) && !isDirectory(Working_Path))
		{
			default_idx = server.values["404"]; /// default error page
			if (!default_idx.empty() && isFile(default_idx))
			{
				Working_Path = default_idx;
				file.close();
				filename = default_idx;
				sent_head = 0;
				tmp_Status_Code = "200";
				Content_Type = GetMimeType(default_idx);
				file.open(default_idx, std::ios::binary);
				Method = "GET";
				return;
			}
			else
				header = ret_header(404, "Not Found", "Not :(:( Found", "text/plain");
		}
		else if (isUpload && (Status_Code == 201 || Status_Code == 200))
			header = ret_header(201, "Ok", "Uploaded Successfully", "text/plain");
		else if(!codeStatusMap[Status_Code].empty())
		{
			default_idx = server.values[_to_string(Status_Code)];
			if (!default_idx.empty() && isFile(default_idx))
			{
				Working_Path = default_idx;
				file.close();
				filename = default_idx;
				sent_head = 0;
				tmp_Status_Code = "200";
				Content_Type = GetMimeType(default_idx);
				file.open(default_idx, std::ios::binary);
				Method = "GET";
				return;
			}
			else
				header = "HTTP/1.1 " + _to_string(Status_Code) + " Bad Request\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: " + _to_string(codeStatusMap[Status_Code].length()) + "\r\n"
					"\r\n"+	codeStatusMap[Status_Code];
		}
		else
			header = ret_header(400, "Bad Request", "Error in upload", "text/plain");
		responseStream.write(header.c_str(), header.length());
		this->end = 1;	return ;
	}
	else if (Method == "GET")
	{
		if ((Status_Code == 200 || tmp_Status_Code == "200")
			&& (isDirectory(Working_Path) && !default_page.empty()))
		{
			Working_Path = default_page;

			file.close();
			filename = default_page;
			Content_Type = GetMimeType(default_page);
			file.open(default_page, std::ios::binary);
			default_page = "";
		}
		if (isFile(Working_Path) && (Status_Code == 200 || tmp_Status_Code == "200"))
		{
			if (!file.is_open())
			{
				header = 
					"HTTP/1.1 404 Not Found\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 9\r\n"
					"\r\n"
					"Not Found";
				body = "";
				responseStream.write(header.c_str(), header.length());
				this->end = 1;
				tmp_Status_Code = "";
				return	;
			}
			else
			{
				if (!sent_head)
				{
					size_t file_size = Calculate_File_Size(file);
					this->Res_Size = file_size;
					header =
						"HTTP/1.1 "+ _to_string(Status_Code) +" OK\r\n"
						"Content-Type: " + Content_Type + "\r\n"
						"Transfer-Encoding: chunked\r\n"
						"Connection: keep-alive\r\n"
						"\r\n";
					sent_head = 1;
					this->bytesRead = 0;
					responseStream.write(header.c_str(), header.length());
					return	;
				}
				else
				{
					file.read(&buffer[0], Chunk_Size); // Read a chunk
					this->current_read = file.gcount();
					if (current_read == 0)
					{
						sent_head = 0;
						tmp_Status_Code = "";
						responseStream.str(""); // Clear previous content
						responseStream.clear();
						std::cerr << "End of file or read error!" << std::endl;
						responseStream << "0\r\n\r\n";
					
						this->end = 1;
						return ;  // End of file or read error
					}
					responseStream << std::hex << current_read << "\r\n";
					responseStream.write(&buffer[0], current_read);
					responseStream << "\r\n";

					this->bytesRead += current_read;
					if ((size_t)this->bytesRead >= this->Res_Size)
					{
						cout << "eeeennnnddddd " << endl;
						responseStream << "0\r\n\r\n";
						tmp_Status_Code = "";
						this->end = 1;
						sent_head = 0;
					}
					return ;
				}
			}
		}
		else if (Status_Code == 200 && this->isLesn && isDirectory(Working_Path))
		{
			DIR* dir = opendir(Working_Path.c_str());
			std::string response = "<h1>Directory Listing: " + Working_Path + "</h1><ul>";
			struct dirent* entry;
			
			if (!dir)
			{
				response = "<h1>404 Not Found</h1><p>Directory not found: " + Working_Path + "</p>";
				header =
				"HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n"
				"Content-Length: "+_to_string(response.length())+"\r\n"
				"\r\n"+ response;
			}
			else
			{
				while ((entry = readdir(dir)) != NULL) 
				{
					// urlFinal
					if (urlFinal == "/")
						response += "<li><a href=\"" + urlFinal + entry->d_name + "\">" + entry->d_name + "</a></li>";
					else
						response += "<li><a href=\"" + urlFinal + "/" + entry->d_name + "\">" + entry->d_name + "</a></li>";
				}
				response += "</ul>";
				closedir(dir);
			}
			header =
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n"
				"Content-Length: "+_to_string(response.length())+"\r\n"
				"\r\n"+ response;
			responseStream.write(header.c_str(), header.length());
			this->end = 1;
			return;
		}
		else if(!codeStatusMap[Status_Code].empty())
		{
			default_idx = server.values[_to_string(Status_Code)];
			if (!default_idx.empty() && isFile(default_idx))
			{
				Working_Path = default_idx;
				file.close();
				filename = default_idx;
				tmp_Status_Code = "200";
				sent_head = 0;
				Content_Type = GetMimeType(default_idx);
				file.open(default_idx, std::ios::binary);
				return;
			}
			else
				header = "HTTP/1.1 " + _to_string(Status_Code) + " Bad Request\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: " + _to_string(codeStatusMap[Status_Code].length()) + "\r\n"
					"\r\n"+	codeStatusMap[Status_Code];
			responseStream.write(header.c_str(), header.length());
			this->end = 1;	return ;
		}
		else
		{
			header = "HTTP/1.1 404 Not Found\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 12\r\n"
					"\r\n"
					"Not :( Found";
			responseStream.write(header.c_str(), header.length());
			this->end = 1;
			return	;
		}
	}
	
}

bool	Response::isDirectory(const std::string& path)
{
	struct stat pathStat;
	if (stat(path.c_str(), &pathStat) != 0)
	{
		// Error occurred (e.g., path does not exist)
		perror("stat");
		return false;
	}
	// S_IFDIR : Represents a directory.
	return (pathStat.st_mode & S_IFMT) == S_IFDIR;
}

bool	Response::isFile(const std::string& path)
{
	struct stat pathStat;
	if (stat(path.c_str(), &pathStat) != 0)
	{
		// Error occurred (e.g., path does not exist)
		perror("stat");
		return false;
	}
	// S_IFREG : Represents a regular file.
	return (pathStat.st_mode & S_IFMT) == S_IFREG;
}
// (pathStat.st_mode & S_IFMT) gives the type of the file (directory, regular file, etc.)


std::string Response::GetMimeType(const std::string& filename)
{
	size_t dotPos = filename.find_last_of('.');
	if (dotPos == std::string::npos) return "text/plain";

	std::string ext = filename.substr(dotPos + 1);
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	// Document Types
	if (ext == "html" || ext == "htm") return "text/html";
	if (ext == "txt") return "text/plain";
	if (ext == "css") return "text/css";
	if (ext == "csv") return "text/csv";
	if (ext == "xml") return "text/xml";
	if (ext == "markdown" || ext == "md") return "text/markdown";

	// Image Types
	if (ext == "jpeg" || ext == "jpg") return "image/jpeg";
	if (ext == "png") return "image/png";
	if (ext == "gif") return "image/gif";
	if (ext == "bmp") return "image/bmp";
	if (ext == "webp") return "image/webp";
	if (ext == "svg") return "image/svg+xml";
	if (ext == "tiff" || ext == "tif") return "image/tiff";

	// Audio Types
	if (ext == "mp3") return "audio/mpeg";
	if (ext == "wav") return "audio/wav";
	if (ext == "ogg") return "audio/ogg";
	if (ext == "aac") return "audio/aac";
	if (ext == "wma") return "audio/x-ms-wma";

	// Video Types
	if (ext == "mp4") return "video/mp4";
	if (ext == "avi") return "video/x-msvideo";
	if (ext == "mov") return "video/quicktime";
	if (ext == "webm") return "video/webm";
	if (ext == "mkv") return "video/x-matroska";

	// Script and Code Types
	if (ext == "js") return "application/javascript";
	if (ext == "json") return "application/json";
	if (ext == "py") return "text/plain";
	if (ext == "cpp" || ext == "cxx") return "text/x-c++src";
	if (ext == "c") return "text/x-csrc";

	// Compressed Files
	if (ext == "zip") return "application/zip";
	if (ext == "rar") return "application/x-rar-compressed";
	if (ext == "7z") return "application/x-7z-compressed";
	if (ext == "gz") return "application/gzip";
	if (ext == "tar") return "application/x-tar";

	// PDF and Office Documents
	if (ext == "pdf") return "application/pdf";
	if (ext == "doc" || ext == "docx") return "application/msword";
	if (ext == "xls" || ext == "xlsx") return "application/vnd.ms-excel";
	if (ext == "ppt" || ext == "pptx") return "application/vnd.ms-powerpoint";

	// Fallback
	// return "application/octet-stream";
	return "text/plain";
}
