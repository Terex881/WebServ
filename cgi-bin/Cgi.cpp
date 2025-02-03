#include "./Cgi.hpp"
#include "../Client.hpp"
#include <exception>
#include <fstream>
#include <unistd.h>

Cgi::Cgi(int socket_fd, string path)
{
	client_socket = socket_fd;
	file_path = path;
}

Cgi::Cgi()
{
	cgi_output = "./cgi_output.txt";
	cgi_error = "./cgi_error.txt";
};

bool fileExists(const std::string& filename)
{
	return (access(filename.c_str(), F_OK) == 0);  // F_OK checks for existence
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

		string key_val = "PATH_INFO="+data->getReq().getRequestData().pathInfo;
		// Child process: execute the CGI script and write output to a file
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
				// exit(1);
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

		string script_name = "SCRIPT_NAME=";
		script_name.append(py_script);

		int i = 5;
		char* envp[6 + data->getReq().getHeaderData().queryStringVec.size()];
		envp[0] = const_cast<char*>("REQUEST_METHOD=GET");
		envp[1] = const_cast<char*>("QUERY_STRING=test");
		envp[2] = const_cast<char*>("SERVER_SOFTWARE=CustomServer/1.0");
		envp[3] = const_cast<char*>(script_name.c_str());
		envp[4] = const_cast<char*>(key_val.c_str());

		std::vector<string>::iterator it = data->getReq().getHeaderData().queryStringVec.begin();
		for (; it != data->getReq().getHeaderData().queryStringVec.end(); it++)
		{
			envp[i++] = const_cast<char*>(it->c_str());
		}
		envp[i] = NULL;

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
			std::cerr << "Failed to register EVFILT_TIMER : 11" << strerror(errno) << std::endl;
		
		EV_SET(&(*data->event), pid, EVFILT_PROC, EV_ADD, NOTE_EXIT, 0, data);
		if (kevent(kq, &(*data->event), 1, NULL, 0, NULL) == -1)
			std::cerr << "Failed to register EVFILT_TIMER : 00 " << strerror(errno) << std::endl;		
	}
}

void Cgi::handleTimeout(pid_t pid, int client_socket, int kq, Client* data)
{

	cout <<BLUE << "Time out " <<RESET<< endl;
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
		// One final wait to clean up zombie
		waitpid(pid, NULL, 0);
	}
	// std::cout << "CGI script timeout, terminating..." << std::endl;
	// EV_SET(&events[1], pid, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
	data->getReq().getRequestData().codeStatus = 504;
	try {
		data->getReq().clean(504, "Gateway Timeout");
	} catch (const exception& e) {
		// Handle the specific exception
	}

	struct kevent event;
	EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, data);
	kevent(kq, &event, 1, NULL, 0, NULL);

	// data->getReq().getHeaderData().filename = "/cgi_output.txt";
}
// EV_SET(&event, pid, EVFILT_PROC, EV_ADD, NOTE_EXIT, 0, (void*)client_socket);
// This line in the parent process is registering an event to monitor the child process. Specifically:

// pid is the process ID of the child that was just created by fork()
// EVFILT_PROC tells kqueue to monitor process-related events
// NOTE_EXIT specifies that we want to be notified when this process exits

// The child process isn't registering anything - it's just being monitored by the parent process. This is important because:

// Parent needs to know when the CGI script (running in child process) finishes
// When the child exits, the parent gets notified via kqueue
// After notification, parent can then read the output file and send it back to the client

// Think of it like this:

// Child process: Just executes the CGI script and writes output to a file
// Parent process: Watches for child to finish using kqueue
// When child finishes: Parent gets notified and can start sending the output

// The event registration is done BY the parent TO monitor the child, not BY the child itsel




// Function to handle the exit of the CGI child process
void	Cgi::handleProcessExit(pid_t pid, int client_socket, int kq, Client* data)
{
	int status;
	int wait_result = waitpid(pid, &status, WNOHANG); // -1

	if (wait_result == -1)
	{
		std::cerr << "waitpid error: " << strerror(errno) << std::endl;
		// Try to kill the process anyway
		kill(pid, SIGTERM);
		usleep(100000); // Wait 100ms
		
		// If still alive, force kill
		if (kill(pid, 0) == 0) {
			kill(pid, SIGKILL);
			// One final wait to clean up zombie
			waitpid(pid, NULL, 0);
		}
	}
	EV_SET(&(*data->event), pid, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
	// kevent(kq, data->events, 2, NULL, 0, NULL);
	if (kevent(kq, &(*data->event), 1, NULL, 0, NULL) == -1) {
		std::cerr << "Failed to register EVFILT_TIMER 0 : " << strerror(errno) << std::endl;
	}
// 	if (kevent(kq, &data->events[1], 1, NULL, 0, NULL) == -1) {
// 	std::cerr << "Failed to register EVFILT_TIMER 1 : " << strerror(errno) << std::endl;
// }
	// sleep(5);
	kill(pid, SIGTERM);
	// exit(1);
	// After CGI script execution, register for writing output to client
	if (WIFEXITED(status))
	{
		cout << "endlddd" << endl;
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
			// data->getReq().getRequestData().isCgi = false;
			data->getReq().getRequestData().codeStatus = 500;
			try {
				data->getReq().clean(500, "Internal Server Erroreeeee");
			} catch (const exception& e) {
				// Handle the specific exception
			}
		}

		struct kevent event;
		EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, data);
		kevent(kq, &event, 1, NULL, 0, NULL);
	}
}
