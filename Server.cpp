#include "Server.hpp"
#include "./Response.hpp"
#include "Client.hpp"
#include "Method/Request/Request.hpp"
// #include "Client.hpp"
#include <ostream>
#include <cstring>
#include <map>

#define MAX_CLIENTS 128
#define BUFFER_SIZE 4096

using std::map;

// typedef struct
// {
// 	int fd;
// 	struct sockaddr_in addr;
// 	int	is_server;
// 	int	is_client;
// 	int sent_head;
// 	string	url;
// 	string	first;
// 	Response rsp;
// 	size_t	bytes_sent;
// 	std::ifstream *file;
// } connection_info;

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

	// std::ofstream outputFile("output_video.py", std::ios::binary); 

	int kq = kqueue();
	if (kq == -1) {
		std::cerr << "kqueue failed" << std::endl;
		exit(1);
	}
	// map<int, connection_info> connections;
	//---------------------------------------------------------------------------S_A_L_A_H----------------------------------------------------------------------------------------
									ofstream ss("tmp.py", ios::app | ios::binary);
										// Client client_obj;
										// client_obj.req_obj = new Request();
																			// Request request_obj;
																			// request_obj.getBody();
																			// request_obj.getHeader();
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------


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
					//-----------------------------------------------------
			clientsMap[new_socket] = Client();
			clientsMap[new_socket].req_obj = new Request();
			clientsMap[new_socket].res_obj = new Response();
					//-----------------------------------------------------
					is_new_connection = true;
					break;
				}
			}
			std::string msg;
			if (!is_new_connection && events[i].filter == EVFILT_READ)
			{
				int client_socket = events[i].ident;
				char buffer[BUFFER_SIZE] = {0};
				int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
				if (bytes_received <= 0) {
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
				} else
				{
					msg.assign(buffer, bytes_received);
//---------------------------------------------------------------------------S_A_L_A_H----------------------------------------------------------------------------------------
									// ss << "[" << client_socket << "]"  << endl;
									// ss << msg;
									// ss << "\n-----------------------------------------------------------------------\n"; ss.flush();
									clientsMap[client_socket].req_obj->request(msg);
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					clientsMap[client_socket].clientData.url = clientsMap[client_socket].req_obj->getElement("uri");;
					clientsMap[client_socket].clientData.sent_head = 0;


					clientsMap[client_socket].clientData.fd = client_socket;
					if (clientsMap[client_socket].clientData.file)
					{
						if (!(clientsMap[client_socket].clientData.file->is_open()))
							clientsMap[client_socket].clientData.file = new std::ifstream("." + clientsMap[client_socket].req_obj->getElement("uri"), std::ios::binary);
					}
					else
						clientsMap[client_socket].clientData.file = new std::ifstream("." + clientsMap[client_socket].req_obj->getElement("uri"), std::ios::binary);
					if(clientsMap[client_socket].req_obj->clientData.requestStat == 2)
					{
						EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, &clientsMap[client_socket]);
						kevent(kq, &event, 1, NULL, 0, NULL);
					}
				}
			}
			else if (events[i].filter == EVFILT_WRITE)
			{
				Client *data = (Client *)events[i].udata;

				int client_socket = events[i].ident;
				// std::stringstream response;
				string wer = data->clientData.url;
				std::string responseStr;

				if (data->clientData.first.empty())
					*data->res_obj = Response(Response::GetMimeType(data->clientData.url), "."+wer, "GET", data->clientData.file, data->clientData.url);
				data->clientData.first = "not empty";
				
				data->res_obj->Res_get_chunk(data->clientData.sent_head);
				responseStr = data->res_obj->responseStream.str();
				// data->res_obj->responseStream
				size_t bytes_sent = send(client_socket, responseStr.data(), responseStr.length(), 0);

				if (bytes_sent < 0 || bytes_sent == SIZE_MAX)
				{
					data->clientData.first = "";
					data->res_obj->bytesRead = 0;
					data->clientData.sent_head = 0;
					delete data->res_obj;
					delete data->req_obj;
					std::cerr << "Send error: " << strerror(errno) 
					<< " (errno: " << errno << ")" << std::endl;
					std::cerr << "send failed" << std::endl;
					clearSocketBuffer(client_socket);
					close(data->clientData.fd);
					EV_SET(&event, data->clientData.fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					if (data->clientData.file)
					{
						if (data->clientData.file->is_open())
							data->clientData.file->close();
						delete data->clientData.file;
						data->clientData.file = NULL;
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
						data->clientData.bytes_sent += bytes_sent;
						if ((size_t)data->res_obj->bytesRead >= data->res_obj->Res_Size || data->res_obj->end)
						{
							delete data->res_obj;
							delete data->req_obj;
							data->clientData.first = "";
							clearSocketBuffer(client_socket);
							data->res_obj->bytesRead = 0;
							data->clientData.sent_head = 0;


							if (data->clientData.file)
							{
								if (data->clientData.file->is_open())
									data->clientData.file->close();
								delete data->clientData.file;
								data->clientData.file = NULL;
							}

							close(client_socket);
							EV_SET(&event, data->clientData.fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
							kevent(kq, &event, 1, NULL, 0, NULL);
						}
				}
			}
		}
	}
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
