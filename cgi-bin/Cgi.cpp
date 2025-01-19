#include "./Cgi.hpp"
#include "../Client.hpp"

Cgi::Cgi(int socket_fd, string path)
{
	client_socket = socket_fd;
	file_path = path;
}

Cgi::Cgi(){};

int	end_header(string header)
{
	if (header.size() > 4 && header.substr(header.size() - 4) == "\r\n\r\n")
		return (1);
	return (0);
}

// Fork and execute the CGI process
void Cgi::execute_script(int client_socket, const std::string& script_path, int kq, Client* data)
{

	pid_t pid = fork();
	if (pid == -1) {
		std::cerr << "Fork failed!" << std::endl;
		close(client_socket);
		return;
	}
	(void)script_path;
	if (pid == 0)
	{
		// Child process: execute the CGI script and write output to a file
		int output_fd = open("cgi_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output_fd == -1) {
			std::cerr << "Failed to open output file" << std::endl;
			exit(1);
		}
		
		// Redirect stdout to output file

		if (dup2(output_fd, STDOUT_FILENO) == -1) {
			std::cerr << "Failed to redirect stdout to file" << std::endl;
			exit(1);
		}
    	close(output_fd);
		const char* py_path = "/usr/local/bin/python3";
		const char* py_script = "/Users/aibn-che/wbw/cgi-bin/script.py";

		char* envp[5];
		envp[0] = const_cast<char*>("REQUEST_METHOD=GET");
		envp[1] = const_cast<char*>("QUERY_STRING=test");
		envp[2] = const_cast<char*>("SERVER_SOFTWARE=CustomServer/1.0");
		envp[3] = const_cast<char*>("param=sddd");
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
		// std::cout << "Executing CGI" << std::endl;
		// Parent process: register the child process for event handling
		EV_SET(&(*data->event), pid, EVFILT_TIMER, EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 10000, data);
		if (kevent(kq, &(*data->event), 1, NULL, 0, NULL) == -1) {
			std::cerr << "Failed to register EVFILT_TIMER : 11" << strerror(errno) << std::endl;
		}
		
		EV_SET(&(*data->event), pid, EVFILT_PROC, EV_ADD, NOTE_EXIT, 0, data);
		if (kevent(kq, &(*data->event), 1, NULL, 0, NULL) == -1) {
			std::cerr << "Failed to register EVFILT_TIMER : 00 " << strerror(errno) << std::endl;
		}
		
		// struct kevent&data->events[1];
		// std::cout << "Executing CGI ..." << std::endl;
	}
}

void Cgi::handleTimeout(pid_t pid, int client_socket, int kq, Client* data)
{
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
	std::cout << "CGI script timeout, terminating..." << std::endl;
	// EV_SET(&events[1], pid, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);

	struct kevent event;
	EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, data);
	kevent(kq, &event, 1, NULL, 0, NULL);

	// data->getReq().getHeaderData().filename = "/cgi_output.txt";
	data->getReq().getRequestData().codeStatus = 404;
	// handleProcessExit(pid, client_socket, kq, data);
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
	// int wait_result = waitpid(pid, &status, WNOHANG); // -1
	// cout << "wait .." << endl;
	// cout << "wait_resutl : " << wait_result << endl;

	    // Try SIGTERM first
    // if (kill(pid, SIGTERM) == 0) {
    //     usleep(100000); // Wait 100ms
        
    //     // If still running, use SIGKILL
    //     if (kill(pid, 0) == 0) {
    //         kill(pid, SIGKILL);
    //     }
    // }
	// if (wait_result == -1)
	// {
	// 	std::cerr << "waitpid error: " << strerror(errno) << std::endl;
	// 	// Try to kill the process anyway
	// 	kill(pid, SIGTERM);
	// 	usleep(100000); // Wait 100ms
		
	// 	// If still alive, force kill
	// 	if (kill(pid, 0) == 0) {
	// 		kill(pid, SIGKILL);
	// 		// One final wait to clean up zombie
	// 		waitpid(pid, NULL, 0);
	// 	}
	// }
	// if (wait_result == 0)
	// {
	// 	// Child is still running, try to terminate it
	// 	kill(pid, SIGTERM);
	// 	int retries = 0;

	// 	while (wait_result == 0 && retries < 10)  // Retry up to 10 times
	// 	{
	// 		std::cout << "1,,,,,,,,, "<< wait_result << std::endl;
	// 		wait_result = waitpid(pid, &status, WNOHANG);
	// 		std::cout << "2,,,,,,,,, "<< wait_result << std::endl;
	// 		sleep(1);  // Wait for 100ms before retry
	// 		retries++;
	// 	}
	// 	if (retries == 10)
	// 	{
	// 		std::cout << "dddddddddddddd:::::::::::::::::" << std::endl;
	// 		kill(pid, SIGTERM);  // Forcefully terminate if it still doesn't exit
	// 		// usleep(100000);  // Give it some time to be killed
	// 	}
	// }

	// struct kevent events[2];
	// EV_SET(&data->events[0], pid, EVFILT_PROC, EV_DELETE, 0, 0, NULL);
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
		// cout << "endlddd" << endl;
		data->getReq().getHeaderData().url = "/cgi_output.txt";
		data->getReq().getRequestData().codeStatus = 200;

		// struct kevent ev;
		// EV_SET(&ev, pid, EVFILT_PROC, EV_DELETE, 0, 0, NULL);
		// // kevent(kq, data->events, 2, NULL, 0, NULL);
		// if (kevent(kq, &ev, 1, NULL, 0, NULL) == -1) {
		// 	std::cerr << "Failed to register EVFILT_PROC 0 : " << strerror(errno) << std::endl;
		// }
		struct kevent event;
		EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, data);
		kevent(kq, &event, 1, NULL, 0, NULL);
	}
}
