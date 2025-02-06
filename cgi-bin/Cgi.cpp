#include "./Cgi.hpp"
#include "../Server/Client.hpp"

Cgi::Cgi(int socket_fd, string path)
{
	client_socket = socket_fd;
	file_path = path;
}

std::string get_current_time_string(){
    struct timeval tv;
    gettimeofday(&tv, NULL);  // Get current time with milliseconds

    struct tm* timeinfo = localtime(&tv.tv_sec);  // Convert to local time

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);  // Format as YYYYMMDD_HHMMSS

    std::ostringstream oss;
    oss << buffer << "_" << std::setw(3) << std::setfill('0') << tv.tv_usec;  // Add milliseconds
    return oss.str();
}

Cgi::Cgi()
{
	cgi_output = "/tmp/.cgi_output_"+ get_current_time_string() +".txt";
	cgi_error = "/tmp/.cgi_error_"+ get_current_time_string() +".txt";
};

void	fill_env(Client *data, char **envp)
{
	int i = 0;
	string script_name = "SCRIPT_NAME=";
	string key_val = "PATH_INFO=" + data->getReq().getRequestData().pathInfo;

	script_name.append(data->getReq().getHeaderData().url.c_str());
	data->getReq().getHeaderData().bigMap["SERVER_NAME"] = "SERVER_NAME="+data->getReq().getHeaderData().bigMap["SERVER_NAME"];
	data->getReq().getHeaderData().bigMap["CONTENT_TYPE"] = "CONTENT_TYPE="+data->getReq().getHeaderData().bigMap["CONTENT_TYPE"];
	data->getReq().getHeaderData().bigMap["REQUEST_METHOD"] = "REQUEST_METHOD="+data->getReq().getHeaderData().bigMap["REQUEST_METHOD"];
	data->getReq().getHeaderData().bigMap["CONTENT_LENGTH"] = "CONTENT_LENGTH="+data->getReq().getHeaderData().bigMap["CONTENT_LENGTH"];
	data->getReq().getHeaderData().bigMap["PATH_INFO"] = key_val;
	data->getReq().getHeaderData().bigMap["SCRIPT_NAME"] = script_name;
	
	envp[i++] = const_cast<char*>(data->getReq().getHeaderData().bigMap["SERVER_NAME"].c_str());
	envp[i++] = const_cast<char*>(data->getReq().getHeaderData().bigMap["CONTENT_TYPE"].c_str());
	envp[i++] = const_cast<char*>(data->getReq().getHeaderData().bigMap["REQUEST_METHOD"].c_str());
	envp[i++] = const_cast<char*>(data->getReq().getHeaderData().bigMap["CONTENT_LENGTH"].c_str());
	envp[i++] = const_cast<char*>(data->getReq().getHeaderData().bigMap["PATH_INFO"].c_str());
	envp[i++] = const_cast<char*>(data->getReq().getHeaderData().bigMap["SCRIPT_NAME"].c_str());

	std::vector<string>::iterator it = data->getReq().getHeaderData().queryStringVec.begin();
	for (; it != data->getReq().getHeaderData().queryStringVec.end(); it++)
	{
		envp[i++] = const_cast<char*>(it->c_str());
	}
	envp[i] = NULL;
}

bool fileExists(const std::string& filename)
{
	return (access(filename.c_str(), F_OK) == 0);
}

// Fork and execute the CGI process
void Cgi::execute_script(int client_socket, int kq, Client* data)
{
	pid_t pid = fork();
	if (pid == -1) {
		std::cerr << "Fork failed!" << std::endl;
		close(client_socket);
		return;
	}
	if (pid == 0)
	{
		int input_fd;
		int error_fd;

		char* envp[10 + data->getReq().getHeaderData().queryStringVec.size()];
		fill_env(data, envp);

		int output_fd = open(cgi_output.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output_fd == -1) {
			std::cerr << "Failed to open output file" << std::endl;
			exit(1);
		}
		error_fd = open(cgi_error.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (error_fd == -1)
		{
			std::cerr << "Failed to open error file" << std::endl;
			close(output_fd);
			exit(1);
		}

		if (!data->getReq().getBodyData().pathFormData.empty())
		{
			input_fd = open(data->getReq().getBodyData().pathFormData.c_str(), O_RDONLY, 0644);
			if (input_fd == -1)
			{
				std::cerr << "Failed to open input file" << std::endl;
				close(output_fd);
				close(error_fd);
				exit(1);
			}
			if (dup2(input_fd, STDIN_FILENO) == -1)
			{
				std::cerr << "Failed to read from file" << std::endl;
				close(input_fd);
				close(output_fd);
				close(error_fd);
				exit(1);
			}
		}
		if (dup2(error_fd, STDERR_FILENO) == -1)
		{
			std::cerr << "Failed to redirect stderr to file" << std::endl;
			if (!data->getReq().getBodyData().pathFormData.empty())
				close(input_fd);
			close(output_fd);
			close(error_fd);
			exit(1);
		}
		if (dup2(output_fd, STDOUT_FILENO) == -1)
		{
			std::cerr << "Failed to redirect stdout to file" << std::endl;
			if (!data->getReq().getBodyData().pathFormData.empty())
				close(input_fd);
			close(output_fd);
			close(error_fd);
			exit(1);
		}
		if (!data->getReq().getBodyData().pathFormData.empty())
			close(input_fd);
		close(output_fd);
		close(error_fd);


		const char* py_path = data->getReq().getRequestData().executable_file.c_str();
		const char* py_script = data->getReq().getHeaderData().url.c_str();

		char* argv[] = {
			const_cast<char*>(py_path),
			const_cast<char*>(py_script),
			NULL
		};
		if (fileExists(py_script))
		{
			execve(py_path, argv, envp);
		}
		std::remove(cgi_output.c_str());
		perror("Execution failed");
		exit(1);
	}
	else
	{
		EV_SET(&(*data->event), pid, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 1000, data);
		if (kevent(kq, &(*data->event), 1, NULL, 0, NULL) == -1)
			std::cerr << "Failed to register EVFILT_TIMER" << std::endl;
		
		EV_SET(&(*data->event), pid, EVFILT_PROC, EV_ADD, NOTE_EXIT, 0, data);
		if (kevent(kq, &(*data->event), 1, NULL, 0, NULL) == -1)
			std::cerr << "Failed to register EVFILT_TIMER" << std::endl;		
	}
}

void Cgi::handleTimeout(pid_t pid, int client_socket, int kq, Client* data)
{
	data->getReq().getRequestData().timeOut = "time_out";
	struct kevent even;
	EV_SET(&even, pid, EVFILT_PROC, EV_DELETE, 0, 0, NULL);
	kevent(kq, &even, 1, NULL, 0, NULL);

	kill(pid, SIGTERM);
	usleep(100000); // Wait 100ms

	// If still alive, force kill
	if (kill(pid, 0) == 0)
	{
		kill(pid, SIGKILL);
		waitpid(pid, NULL, 0);
	}

	try {
		data->getReq().clean(504, "Gateway Timeout");
	} catch (const std::exception& e) {
	}

	struct kevent event;
	EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, data);
	kevent(kq, &event, 1, NULL, 0, NULL);
}

void	Cgi::handleProcessExit(pid_t pid, int client_socket, int kq, Client* data)
{
	int status;
	int wait_result = waitpid(pid, &status, WNOHANG);

	if (wait_result == -1)
	{
		kill(pid, SIGTERM);
		usleep(100000);
		
		if (kill(pid, 0) == 0)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
		}
	}
	data->getReq().getRequestData().timeOut = "";
	EV_SET(&(*data->event), pid, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
	kevent(kq, &(*data->event), 1, NULL, 0, NULL);
	kill(pid, SIGTERM);

	if (WIFEXITED(status))
	{
		data->getReq().getHeaderData().url = cgi_output;
		data->getReq().getRequestData().codeStatus = 200;
		std::ifstream f(cgi_error);
		if (f.is_open() && data->getRes().Calculate_File_Size(f) != 0)
		{
			f.close();
			data->getReq().getHeaderData().url = cgi_error;
			data->getReq().getRequestData().cgiError = "yes";
		}
		if (!fileExists(cgi_output))
		{
			data->getReq().getRequestData().codeStatus = 500;
			try {
				data->getReq().clean(500, "Internal Server Error");
			} catch (const std::exception& e) {
			}
		}

		struct kevent event;
		EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, data);
		kevent(kq, &event, 1, NULL, 0, NULL);
	}
}
