#include "Server.hpp"
#include "Delete.hpp"
#include <ostream>
#include <cstring>
#include <vector>
#include "./Response.hpp"
#include "Request/Request.hpp"

#define MAX_CLIENTS 128
#define BUFFER_SIZE 4096

typedef struct
{
	int fd;
	struct sockaddr_in addr;
	int	is_server;
	int	is_client;
	string	request;
	size_t	bytes_sent;
	std::ifstream *file;
} connection_info;

// Function to clear socket's incoming buffer
void clearSocketBuffer(int socket) {
    char buffer[1024];
    ssize_t bytes_read;

    // Set socket to non-blocking mode temporarily
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);

    // Read and discard all pending data
    while ((bytes_read = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        // Discard the data
    }

    // Restore original socket flags
    fcntl(socket, F_SETFL, flags);
}

void Server::ft_start(int size, int *fd) {

	std::ofstream outputFile("output_video.py", std::ios::binary); 

	int kq = kqueue();
	if (kq == -1) {
		std::cerr << "kqueue failed" << std::endl;
		exit(1);
	}
	Request R1;
	connection_info connections[MAX_CLIENTS];
	memset(connections, 0, sizeof(connections));

	// Set up events for each server listening socket
	struct kevent event;
	for (int i = 0; i < size; ++i) {
		EV_SET(&event, fd[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
			std::cerr << "kevent failed for server " << i << std::endl;
			exit(1);
		}
	}

	// Array to store client socket file descriptors
	int client_sockets[MAX_CLIENTS] = {0};
	std::string client_buffers[MAX_CLIENTS]; // Store data to be sent

	while (true) {
		struct kevent events[MAX_CLIENTS];
		int n = kevent(kq, NULL, 0, events, MAX_CLIENTS, NULL); // Wait for events
		if (n < 0) {
			std::cerr << "kevent failed" << std::endl;
			exit(1);
		}

		for (int i = 0; i < n; ++i) {
			bool is_new_connection = false;
			connection_info *data = (connection_info *)events[i].udata;

			// Check if the event corresponds to one of the server's listening sockets
			for (int j = 0; j < size; ++j) {
				if ((int)events[i].ident == fd[j] && events[i].filter == EVFILT_READ) {
					// Accept new client connection
					struct sockaddr_in client_addr;
					socklen_t addrlen = sizeof(client_addr);
					int new_socket = accept(fd[j], (struct sockaddr*)&client_addr, &addrlen);
					if (new_socket < 0) {
						std::cerr << "accept failed" << std::endl;
						continue;
					}

					std::cout << "New client connected on server " << j << ": " << new_socket << std::endl;

					// Add the new socket to the kqueue for future reading events
					EV_SET(&event, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					// Optionally, store the socket in an array for further management
					for (int k = 0; k < MAX_CLIENTS; ++k) {
						if (client_sockets[k] == 0) {
							client_sockets[k] = new_socket;
							break;
						}
					}

					is_new_connection = true;
					break;
				}
			}

			// If it's not a new connection, it's a client sending data
			std::string msg;
			if (!is_new_connection && events[i].filter == EVFILT_READ)
			{
				int client_socket = events[i].ident;
				char buffer[BUFFER_SIZE] = {0};
				int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
				cout << "\n*******************************************\n" << buffer << "\n***********************************************\n";
				if (bytes_received <= 0) {
					// Client has closed the connection or error occurred
					std::cout << "Client " << client_socket << " disconnected or error occurred." << std::endl;
					close(client_socket);
					EV_SET(&event, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					// Remove from the client list
					for (int j = 0; j < MAX_CLIENTS; ++j) {
						if (client_sockets[j] == client_socket) {
							client_sockets[j] = 0;
							break;
						}
					}
				} else{
					// Handle incoming message (echo or any other processing)
					

					msg.assign(buffer, bytes_received);

					outputFile << "Received from client: " << msg<< std::endl;
					// if (msg.find("POST") != std::string::npos) {

					// 	//salah
					R1.request(msg);
					string sas = msg;
					/////////////////////////Data-Associated-With-The-Current-Event///////////////////////////////////
					size_t	first_pos = sas.find(' ');

					if (first_pos != string::npos)
					{
						size_t	second_pos = sas.find(' ', first_pos + 1);
						sas = sas.substr(first_pos + 1, second_pos - first_pos - 1);
					}

					outputFile << "---------++++++++++++++ |||| RequestT |||| +++***********---------------" << std::endl;
					outputFile << msg << std::endl;

					outputFile << sas << std::endl;
					connections[client_socket].request = R1.getElement("path");

					outputFile << "Path :: " << connections[client_socket].request << std::endl;

					connections[client_socket].fd = client_socket;
					if (connections[client_socket].file)
					{
						if (!(connections[client_socket].file->is_open()))
							connections[client_socket].file = new std::ifstream("."+R1.getElement("path"), std::ios::binary);
					}
					else
						connections[client_socket].file = new std::ifstream("."+R1.getElement("path"), std::ios::binary);
					outputFile << "---------++++++++++++++ |||| Request |||| +++***********---------------\n\n\n\n\n\n" << std::endl;
					/////////////////////////Data-Associated-With-The-Current-Event///////////////////////////////////

					// connections[client_socket].file = new std::ifstream("."+sas, std::ios::in);
					// 	// Prepare a response to send
					// 	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 11\r\n\r\nHello world";
					// 	client_buffers[client_socket] = response;

					// 	// Add a write event to the kqueue
					EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, &connections[client_socket]);
					kevent(kq, &event, 1, NULL, 0, NULL);
					// } else
					//  if (msg.find("GET") != std::string::npos) {
					// 	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world3";
					// 	send(client_socket, response.c_str(), response.size(), 0);
					// }
				}
			}
			else if (events[i].filter == EVFILT_WRITE)
			{
				int client_socket = events[i].ident;

				outputFile << "*** FD = " << client_socket << "***" << std::endl;
				outputFile << "URL : " << connections[client_socket].request << std::endl;
				// outputFile << "URL : " << data->request << std::endl;
				string wer = data->request;
				Response res(200, 10, Response::GetMimeType(data->request), "OK", "."+wer, "GET", connections[client_socket].file, client_socket, data->request);


				std::stringstream response;
				res.Res_get_chunk(response);
				std::string responseStr = response.str();
				const char *bfff = responseStr.data();
				(void)bfff;
				outputFile << "\n\n\n---------++++++++++++++Responset+++***********---------------" << std::endl;
				// outputFile << responseStr << std::endl;
				
				// outputFile.write(responseStr.data(), responseStr.length());

				outputFile << "response Length = " << responseStr.length() << "\n"; 

				// Careful sending
				size_t bytes_sent = send(client_socket, responseStr.data(), responseStr.length(), 0);


				outputFile << "** Bytes_sent = " << bytes_sent << std::endl; // 18446744073709551615
				outputFile << "---------++++++++++++++Response+++***********---------------\n\n\n\n\n\n" << std::endl;

				if (bytes_sent < 0 || bytes_sent == SIZE_MAX)
				{
					// Log specific error
					std::cerr << "Send error: " << strerror(errno) 
					<< " (errno: " << errno << ")" << std::endl;
					std::cerr << "send failed" << std::endl;
					clearSocketBuffer(client_socket);
					close(client_socket);
					EV_SET(&event, client_socket, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					if (connections[client_socket].file)
					{
						if (connections[client_socket].file->is_open())
							connections[client_socket].file->close();
						delete connections[client_socket].file;
						connections[client_socket].file = NULL;
					}
					// Remove from the client list
					for (int j = 0; j < MAX_CLIENTS; ++j)
					{
						if (client_sockets[j] == client_socket)
						{
							client_sockets[j] = 0;
							break;
						}
					}
				}
				else
				{
						outputFile << "\n\n\n---------++++++++++++++Sent t+++***********---------------" << std::endl;
						data->bytes_sent += bytes_sent;
						outputFile << "-- Total Read -- " << data->bytes_sent <<  std::endl;
						outputFile << "-- current Read -- " << bytes_sent <<  std::endl;
						outputFile << "###0000##### file_size = " <<  res.Res_Size << " ######0000####  " << res.bytesRead << std::endl;

						if ((size_t)res.bytesRead >= res.Res_Size || res.end)
						{
							clearSocketBuffer(client_socket);

							outputFile << "------------- End ---------------" << std::endl;
							if (connections[client_socket].file)
							{
								if (connections[client_socket].file->is_open())
									connections[client_socket].file->close();
								delete connections[client_socket].file;
								connections[client_socket].file = NULL;
							}
							
							close(client_socket);
							EV_SET(&event, data->fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
							kevent(kq, &event, 1, NULL, 0, NULL);
						
						}
						outputFile << "---------++++++++++++++Sent+++***********---------------\n\n\n\n\n\n" << std::endl;
				}
			}
		}
	}

	// Cleanup: Close all server and client connections
	for (int i = 0; i < size; ++i) {
		close(fd[i]);
	}
}

Server::Server(std::string host, int port)
{
	this->host = host;
	this->port = port;
}

Server::Server() {
	this->host = "127.0.0.1";
	this->port = 8080;
}
																																									
Server::~Server() {
	std::cout << "good bye...\n";
}




int Server::ft_server_init() {

	if (this->port < 1024 || this->port > 65535) {
		std::cout << "Error port number\n";
		exit(1);
	}

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		std::cout << "socket failed" << std::endl;
		exit(1);
	}
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	// Enable SO_REUSEADDR to allow quick reuse of the port
	int opt = 1;
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
		close(server_fd);
		exit(1);
	}

	int sendbuf_size = 256 * 1024;  // 256KB
	setsockopt(server_fd, SOL_SOCKET, SO_SNDBUF, &sendbuf_size, sizeof(sendbuf_size));

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(this->host.c_str()); // by default Binding to localhost
	server_addr.sin_port = htons(this->port); // by default Port 8080

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		std::cout << "bind failed" << std::endl;
		close(server_fd);
		exit(1);
	}

	std::cout << "Server is listening on " << this->host << ":" << this->port << std::endl;
	if (listen(server_fd, MAX_CLIENTS) == -1) {
		std::cout << "listen failed" << std::endl;
		close(server_fd);
		exit(1);
	}

	return (server_fd);
}
