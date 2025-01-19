#include "./Response.hpp"
#include "Method/Request/Request.hpp"
#include <cstddef>
#include <string>

size_t	Calculate_File_Size(std::ifstream &file)
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
					string working_path, string method, string Url, int codeStatus, bool isLesn, string filename):Chunk_Size(1024) // 8 KB chunks 8192
{
	this->Content_Type = content_type;
	this->Working_Path = working_path;
	this->Method =	method;
	this->current_read = 0;
	this->Url = Url;
	this->end = 0;
	this->Status_Code = (size_t)codeStatus;
	this->isLesn = isLesn;

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
			filename(other.filename)
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

void	Response::Res_get_chunk(int &sent_head)
{
	std::vector<char> buffer(Chunk_Size, 0);
	responseStream.str(""); // Clear previous content
	responseStream.clear(); // Clear any error flags



	if (Method == "GET")
	{
		if (Status_Code != 200)
		{
			std::cout << Status_Code << std::endl;
			
				header = 
					"HTTP/1.1 " + std::to_string(Status_Code) +" Internal Error\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 14\r\n"
					"\r\n"
					"Internal Error";
				body = "";
				responseStream.write(header.c_str(), header.length());
				this->end = 1;
				return	;
		}
		else if (isFile(Working_Path))
		{
			if (!file.is_open())   ////////////// SEGFAULT
			{
				std::cout << "Not Open : " << filename << std::endl;
				header = 
					"HTTP/1.1 404 Not Found\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 9\r\n"
					"\r\n"
					"Not Found";
				body = "";
				responseStream.write(header.c_str(), header.length());
				this->end = 1;
				return	;
			}
			else
			{
				if (!sent_head)
				{
					size_t file_size = Calculate_File_Size(file);
					this->Res_Size = file_size;
					// std::cout << "######## file_size = " <<  file_size << " ##########" << std::endl;
					header =
						"HTTP/1.1 200 OK\r\n"
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
					file.read(buffer.data(), Chunk_Size); // Read a chunk
					this->current_read = file.gcount();
					if (current_read == 0)
					{
						sent_head = 0;
						responseStream.str(""); // Clear previous content
						responseStream.clear();
						std::cerr << "End of file or read error!" << std::endl;
						responseStream.write("0\r\n\r\n", 6);
						return ;  // End of file or read error
					}
					responseStream << std::hex << current_read << "\r\n";
					responseStream.write(buffer.data(), current_read);
					responseStream << "\r\n";
					this->bytesRead += current_read;
					if ((size_t)this->bytesRead >= this->Res_Size)
					{
						responseStream.write("0\r\n\r\n", 6);
						this->end = 1;
						sent_head = 0;
					}
					return ;
				}
			}
		}
		else if (this->isLesn && isDirectory(Working_Path))
		{
			// Working_Path = "/Users/aibn-che/wbw/static";
			cout << Working_Path << "   " << isLesn << endl;
			DIR* dir = opendir(Working_Path.c_str());
			std::string response = "<h1>Directory Listing: " + Working_Path + "</h1><ul>";
			struct dirent* entry;
			
			if (!dir)
			{
				response = "<h1>404 Not Found</h1><p>Directory not found: " + Working_Path + "</p>";
				header =
				"HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n"
				"Content-Length: "+std::to_string(response.length())+"\r\n"
				"\r\n"+ response;
			}
			else
			{
				while ((entry = readdir(dir)) != NULL) 
				{
					// response += "<li><a href=\"" + Working_Path + "/" + entry->d_name + "\">" + entry->d_name + "</a></li>";
					response += "<li><a href=\"" + std::string(entry->d_name) + "\">" + entry->d_name + "</a></li>";
				}
				response += "</ul>";
				closedir(dir);
			}
			header =
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n"
				"Content-Length: "+std::to_string(response.length())+"\r\n"
				"\r\n"+ response;
			responseStream.write(header.c_str(), header.length());
			this->end = 1;
		}
		else if (isDirectory(Working_Path))
		{
			body =
				"<html>"
					"<head>"
						"<title>default page</title>"
					"</head>"
					"<body>"
						"<h1>Default page</h1>"
					"</body>"
				"</html>";
			header =
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n"
				"Content-Length: "+std::to_string(body.length())+"\r\n"
				"\r\n"+ body;
			responseStream.write(header.c_str(), header.length());
			this->end = 1;
		}
		else
		{
			cout << Working_Path << "   " << isLesn << endl;
			header =
					"HTTP/1.1 404 Not Found\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 9\r\n"
					"\r\n"
					"Noot Found";
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
	if (ext == "py") return "application/x-python-code";
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
