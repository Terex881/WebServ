#include "./Cgi.hpp"
#include <fcntl.h>

Cgi::Cgi(int socket_fd, string path)
{
	client_socket = socket_fd;
	file_path = path;
}

int	end_header(string header)
{
	if (header.size() > 4 && header.substr(header.size() - 4) == "\r\n\r\n")
		return (1);
	return (0);
}

bool	Cgi::execute_script()
{
	int	pid;
	int fd[2];
	char bff[1100];
	int end = 0;
	if (pipe(fd) < 0)
	{
		perror("Pipe creation failed");
		return false;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return false;
	}
	if (!pid)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
        	perror("dup2 failed"), exit(1);
		close(fd[1]);

		// const char* php_path = "/usr/bin/php";
		// const char* php_script = "/Users/aibn-che/web-serve/cgi-bin/index.php";
		
		const char* py_path = "/usr/local/bin/python3";
		const char* py_script = "/Users/aibn-che/web-serve/cgi-bin/script.py";

		char* envp[5];
		envp[0] = const_cast<char*>("REQUEST_METHOD=GET");
		envp[1] = const_cast<char*>("QUERY_STRING=test");
		envp[2] = const_cast<char*>("SERVER_SOFTWARE=CustomServer/1.0");
		envp[3] = const_cast<char*>("param=hello");
		envp[4] = NULL;

		char* argv[] = {
			const_cast<char*>(py_path),
			const_cast<char*>(py_script),
			NULL
		};

		execve(py_path, argv, envp);
		perror("Execution failed");
		exit(1);
	}
	else
	{
		content_len = 0;
		int status;
		// struct timeval timeout;

		// Make a file descriptor (e.g., a socket) non-blocking, so operations like read or write return immediately if no data is available.
		int flags = fcntl(fd[0], F_GETFL, 0);
		fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);
		waitpid(pid, &status, 0);
		response = "";
		size_t rr;

		while (true)
		{
				rr = read(fd[0], bff, 1);
				if (rr <= 0)
				{
					perror("Error Reading File\n");
					return 0;
				}
				if (rr >= 1100)
					break ;
				bff[rr] = '\0';
				if (end)
					content_len += rr;
				response += bff;
				if (!end && end_header(response))
					end = 1;
				memset(bff, '\0', sizeof(bff));
		}
		// content_len -= 28;
		close(fd[1]);
		close(fd[0]);
		return WIFEXITED(status) && WEXITSTATUS(status) == 0;
	}
}





	/////////// ______________________ Cgi ______________________ /////(Test)//////
	
					// Cgi gi(0, "/Users/aibn-che/web-serve/cgi-bin/index.php");
					// gi.execute_script();


					// std::ostringstream response_headers;

					// response_headers << "HTTP/1.1 200 OK\r\n"
					// 	<< "Server: TTM \r\n"
					// 	<< "Content-Length: " << gi.content_len << "\r\n"
					// 	<< "Connection: close\r\n"
					// 	<< gi.response;

					// int f = 0;
					// string header = response_headers.str().substr(0, gi.content_len);
					// std::stringstream re(response_headers.str());
					// std::map<string, string> key_val;
					// string word;
					// while (re >> word)
					// {
					// 	if (word == "HTTP/1.1")
					// 		key_val["protocol"] = word;
					// 	else if (word == "200")
					// 		key_val["status_code"] = word;
					// 	else if (word == "OK")
					// 		key_val["message"] = word;
						
					// }
					// std::map<string, string>::iterator it;

					// cout << "-----------------------------------------\n" << endl;
					// send(client_socket, response_headers.str().c_str(), response_headers.str().length(), 0);

					// // Close the client connection after serving
					// close(client_socket);
					// EV_SET(&event, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					// kevent(kq, &event, 1, NULL, 0, NULL);
/////////// ______________________ Cgi ______________________ ///////////