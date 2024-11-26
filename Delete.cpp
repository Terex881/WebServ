#include "Delete.hpp"

//config file check if the allowed is there for delete otherwise 405

Delete::Delete()
{
	this->file_to_deleted = "";
	this->response = "";
	this->flag_response = -1337;
}

Delete::Delete(std::string file)
{
	this->file_to_deleted = file;
	this->flag_response = -1337;
	this->response = "";
}

int Delete::Can_Be_Deleted()
{
	struct stat file_info;

    // Check if file exists and retrieve file stats
    if (stat(this->file_to_deleted.c_str(), &file_info) != 0) {
        std::cout << "File does not exist or could not be accessed: " << this->file_to_deleted << std::endl;
		this->flag_response = 404;
		this->response = "HTTP/1.1 404 Not Found\r\n"
                                          "Content-Type: text/plain\r\n"
                                          "Content-Length: " + std::to_string(11) + "\r\n"
                                          "\r\n" +  // Blank line separating headers and body
                                          "Error The requested file could not be found on the server.";
        return 0;
    }

    // Check if the file is writable (has write permission for the user)
    if ((file_info.st_mode & S_IWUSR) == 0) {
        std::cout << "No write permission to delete the file: " << this->file_to_deleted << std::endl;
		this->flag_response = 403;
		this->response = "HTTP/1.1 403 Forbidden\r\n"
                                          "Content-Type: text/plain\r\n"
                                          "Content-Length: " + std::to_string(11) + "\r\n"
                                          "\r\n" +  // Blank line separating headers and body
                                          "Forbidden You do not have permission to delete this file.";
        return 0;
    }
    return 1;
}


void Delete::Delete_File()
{
	// check if the file can be deleted
	if (this->Can_Be_Deleted())
	{
		if (remove(this->file_to_deleted.c_str()) == 0)
		{
			std::cout << "file deleted sucessfully\n";
			this->flag_response = 200;
			this->response = "HTTP/1.1 200 OK\r\n"
                                          "Content-Type: text/plain\r\n"
                                          "Content-Length: " + std::to_string(11) + "\r\n"
                                          "\r\n" +  // Blank line separating headers and body
                                          "file is deleted";
		}
		else
		{
			std::cout << "failed deleting the file\n";
			this->flag_response = 500;
			this->response = "HTTP/1.1 500 Internal Server Error\r\n"
                                          "Content-Type: text/plain\r\n"
                                          "Content-Length: " + std::to_string(11) + "\r\n"
                                          "\r\n" +  // Blank line separating headers and body
                                          "Error Internal Server Error";
		}
	}
	else 
	{
		std::cout<< "unable to delelte the file\n";
	}
}

Delete::~Delete()
{
	
}