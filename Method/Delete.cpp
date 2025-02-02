#include "Delete.hpp"
#include "Request/Request.hpp"

//config file check if the allowed is there for delete otherwise 405

Delete::Delete()
{
	this->file_to_deleted = "";
	// this->response = "";
	// this->path_config = "";
	// this->delete_config = 0;
	// this->flag_response = -1337;
	// this->cgi_ = 0;
	// cout <<  "343434534534534545345343455345" << endl;
}

Delete::Delete(std::string file, std::string path, int delete_con, int cgi)
{
	this->file_to_deleted = file;
	this->flag_response = -1337;
	this->response = "";
	this->path_config = path;
	this->delete_config = delete_con;
	this->cgi_ = cgi;
}

int Delete::Can_Be_Deleted()
{

	if (this->cgi_ == 1)
	{
		this->flag_response = 501;
		this->response = "HTTP/1.1 501 Not Implemented\r\n"
										"Content-Type: text/plain\r\n"
										"Content-Length: " + _to_string(35) + "\r\n"
										"\r\n" +  // Blank line separating headers and body
										"Not Implemented to delete this file";
		return 0;
	}
	if (this->delete_config == 0)
	{
		this->flag_response = 405;
		this->response = "HTTP/1.1 405 Method Not Allowed\r\n"
										"Content-Type: text/plain\r\n"
										"Content-Length: " + _to_string(57) + "\r\n"
										"\r\n" +  // Blank line separating headers and body
										"Forbidden You do not have permission to delete this file.";
		return 0;
	}
	else
	{
		// struct stat file_info;

		//check if it is a directory
		DIR *dir = opendir((this->path_config + this->file_to_deleted).c_str());
		if (dir != NULL)
		{
			std::cout << "It's a directory!" << std::endl;
			closedir(dir);
			this->flag_response = 403;
			this->response = "HTTP/1.1 403 Forbidden\r\n"
											"Content-Type: text/plain\r\n"
											"Content-Length: " + _to_string(57) + "\r\n"
											"\r\n" +  // Blank line separating headers and body
											"Forbidden You do not have permission to delete directory.";
			return 0;
		}
		else
		{
			std::string absolute = this->path_config + this->file_to_deleted;
			struct stat buffer;
    		// stat() returns 0 if the file exists, -1 if it doesn't
			if (stat(absolute.c_str(), &buffer) == 0)
			{
				if ((buffer.st_mode & S_IWUSR) == 0) {
					std::cout << "No write permission to delete the file: " << this->file_to_deleted << std::endl;
					this->flag_response = 403;
					this->response = "HTTP/1.1 403 Forbidden\r\n"
													"Content-Type: text/plain\r\n"
													"Content-Length: " + _to_string(57) + "\r\n"
													"\r\n" +  // Blank line separating headers and body
													"Forbidden You do not have permission to delete this file.";
					return 0;
				}
				return (1);
			}
		}
	}
	cout << "here 1 ---------------------------------\n";
    return 2;
}


void Delete::Delete_File()
{
	// check if the file can be deleted
	int a = this->Can_Be_Deleted();
	if (a == 1)
	{
		if (remove((this->path_config + this->file_to_deleted).c_str()) == 0)
		{
			std::cout << "file deleted sucessfully\n";
			this->flag_response = 200;
			this->response = "HTTP/1.1 200 OK\r\n"
                                          "Content-Type: text/plain\r\n"
                                          "Content-Length: " + _to_string(15) + "\r\n"
                                          "\r\n" +  // Blank line separating headers and body
                                          "file is deleted";
		}
	}
	else if (a == 2)
	{
		cout << "here 2 ---------------------------------\n";
		std::cout << "failed deleting the file\n";
		this->flag_response = 400;
		this->response = "HTTP/1.1 500 Internal Server Error\r\n"
										"Content-Type: text/plain\r\n"
										"Content-Length: " + _to_string(21) + "\r\n"
										"\r\n" +  // Blank line separating headers and body
										"Internal Server Error";
	}
}

Delete::~Delete()
{
	
}