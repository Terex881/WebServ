#include "./Response.hpp"

// int Response::sent_head = {0};
int Response::sent_head[MAX_CLIENTS] = {0};
int Response:: sre = 0;
size_t	Calculate_File_Size(std::ifstream &file)
{
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	return fileSize;
}

Response::Response(size_t status_code, size_t content_length, string content_type,\
					string reason_phrase, string working_path, string method, std::ifstream *file, int idx, string Url):Chunk_Size(1024) // 8 KB chunks 8192
{
	this->Status_Code = status_code;
	this->Content_Length = content_length;
	this->Content_Type = content_type;
	this->Reason_Phrase = reason_phrase;
	this->Working_Path = working_path;
	this->Method =	method;
	this->current_read = 0;
	this->file = file;
	this->idx = idx;
	this->Url = Url;
	this->end = 0;

	// this->file = new std::ifstream(Url, std::ios::binary);
	// char buffer[20];
    // file->read(buffer, sizeof(buffer));

    // // Get the current offset in the file (after reading some data)
    // std::streampos currentOffset = file->tellg();

    // std::cout << "Current file offset: " << currentOffset << " bytes\n";
    
    // // Clean up and close the file
    // file->close();

	// exit(1);

	// header =  "HTTP/1.1 " + std::to_string(Status_Code) + " " + Reason_Phrase +"\r\n"
	// 						"Content-Type: "+ Content_Type +"\r\n"
	// 						"Content-Length: " + std::to_string(content_length) + "\r\n"
	// 						// "Accept-Ranges: bytes\r\n"
	// 						"Accept-Ranges: bytes\r\n"
	// 						"Cache-Control: no-store, no-cache, must-revalidate, max-age=0\r\n"
	// 						"Keep-Alive: timeout=5\r\n"
	// 						"Connection: Keep-Alive \r\n"
	// 						"\r\n";  // Blank line separating headers and body


	// ****** index.html, directory_listing, redirection ********
	// ----------------------------------------------------------
}

void	Response::Res_get_chunk(std::stringstream &responseStream)
{
	// char buffer[Chunk_Size]; // 8192
	responseStream.str(""); // Clear previous content
	responseStream.clear(); // Clear any error flags
	if (Method == "GET")
	{
			std::cout << " -------------------==========1===============     ------------------- " << std::endl;
			std::cout << " Working _ Path " << Working_Path.c_str() << std::endl;
		if (isFile(Working_Path))
		{
			std::cout << " -------------------===========2==============     ------------------- " << std::endl;
			// std::ifstream file(Working_Path, std::ios::binary);
			if (!(file)->is_open())
			{
				std::cout << "Not Open " << std::endl;
				header = 
					"HTTP/1.1 404 Not Found\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 9"
					"\r\n"
					"Not Found";
				body = "";
				responseStream.write(header.c_str(), header.length());
				end = 1;
				return	;

			}
			else
			{
				if (!sent_head[idx])
				{
					size_t file_size = Calculate_File_Size(*file);
					this->Res_Size = file_size;
					std::cout << "######## file_size = " <<  file_size << " ##########" << std::endl;
					header =
						"HTTP/1.1 200 OK\r\n"
						"Content-Type: " + Content_Type + "\r\n"
						"Content-Length: " + std::to_string(file_size) + "\r\n"
						"Accept-Ranges: bytes\r\n"
						"Connection: close\r\n"
						"\r\n";

						// "Keep-Alive: timeout=3, max=10\r\n"
					std::ofstream outputFile("output_video", std::ios::binary); 
					outputFile.write(header.data(), header.length());
					this->sent_head[idx] = 1;
					this->bytesRead = 0;
			
					responseStream.write(header.c_str(), header.length());
					return	;
				}
				else
				{
					std::cout << "________________ Reading Body _________" << std::endl;

					// memset(buffer, '\0', sizeof(buffer));
					(*file).read(buffer, Chunk_Size); // Read a chunk
					this->current_read = (*file).gcount();
					std::cout << " bytesRead = " << current_read <<" --- " << std::endl;
					if (current_read == 0)
					{
						std::cerr << "End of file or read error!" << std::endl;
						memset(buffer, '\0', sizeof(buffer));

						responseStream.write(buffer, current_read);
						return ;  // End of file or read error
					}
					buffer[current_read] = '\0';
					responseStream.write(buffer, current_read);
					this->bytesRead += current_read;
					if ((size_t)this->bytesRead >= this->Res_Size)
					{
						this->file->close();
						sent_head[idx] = 0;
					}
					std::cout << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
					std::cout << "===== ==== bytes_read " << current_read << std::endl;
					std::cout << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
					return ;
				}

				// the main issue is with IO, i have a single socket (represent a single server), and this single server
				// might get a lot of request, so the main question is, how can i handle mutiple requests ?
				
				std::cout << buffer << std::endl; // line 112
				// std::cout << ";;;;;;;;;;;;;;"<< buffer.c_str().length() << ";;;;;;;;;;;;;;;;;;;;" << std::endl;
				std::cout << " enddddd " << std::endl;
				// file.close();
			}
		}
		// else if (isDirectory(Working_Path))
		// {

		// }
		else
		{
			header =
					"HTTP/1.1 404 Not Found\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 9"
					"\r\n"
					"Not Found";
			responseStream.write(header.c_str(), header.length());
			end = 1;
			return	;
		}
		// close(socket);
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
