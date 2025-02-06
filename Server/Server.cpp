#include "Server.hpp"
#include "../Response.hpp"
#include "Client.hpp"
#include "../Method/Request/Request.hpp"

#define MAX_CLIENTS 128
#define BUFFER_SIZE 4096 * 2

void Server::ft_start(int size, int *fd)
{
	int kq = kqueue();
	if (kq == -1)
	{
		std::cerr << "kqueue failed" << std::endl;
		exit(1);
	}

	struct kevent event;
	for (int i = 0; i < size; ++i) {
		EV_SET(&event, fd[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
			std::cerr << "kevent failed for server " << i << std::endl;
			exit(1);
		}
	}

	int client_sockets[MAX_CLIENTS] = {0};
	std::string client_buffers[MAX_CLIENTS];

	while (true) {
		struct kevent events[MAX_CLIENTS];
		int n = kevent(kq, NULL, 0, events, MAX_CLIENTS, NULL);
		if (n < 0) {
			std::cerr << "kevent failed" << std::endl;
			exit(1);
		}


		for (int i = 0; i < n; ++i) {
			bool is_new_connection = false;

			for (int j = 0; j < size; ++j) {
				if ((int)events[i].ident == fd[j] && events[i].filter == EVFILT_READ) {
					struct sockaddr_in client_addr;
					socklen_t addrlen = sizeof(client_addr);
					int new_socket = accept(fd[j], (struct sockaddr*)&client_addr, &addrlen);
					if (new_socket < 0) {
						std::cerr << "accept failed" << std::endl;
						continue;
					}
					std::cout << "New client connected on server " << j << ": " << new_socket << std::endl;
					EV_SET(&event, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					for (int k = 0; k < MAX_CLIENTS; ++k) {
						if (client_sockets[k] == 0) {
							client_sockets[k] = new_socket;
							break;
						}
					}
							clientsMap[new_socket] = Client();
							clientsMap[new_socket].event = &event;
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

					for (int j = 0; j < MAX_CLIENTS; ++j) {
						if (client_sockets[j] == client_socket) {
							client_sockets[j] = 0;
							break;
						}
					}
				} else
				{
					msg.assign(buffer, bytes_received);
					try
					{
						clientsMap[client_socket].getReq().request(msg);
					}
					catch(const std::exception& e)
					{
					}
									
					clientsMap[client_socket].getReq().getRequestData().sent_head = 0;
					clientsMap[client_socket].getReq().getRequestData().fd = client_socket;
					if (clientsMap[client_socket].getReq().getRequestData().isCgi
                        && clientsMap[client_socket].getReq().getRequestData().requestStat == 2
                        && (clientsMap[client_socket].getReq().getHeaderData().requestMethod == "GET"
                        || clientsMap[client_socket].getReq().getHeaderData().requestMethod == "POST")
                        && clientsMap[client_socket].getReq().getRequestData().codeStatus != 405)
					{
						clientsMap[client_socket].getCgi().execute_script(client_socket, kq, &clientsMap[client_socket]);
					}
					else if(clientsMap[client_socket].getReq().getRequestData().requestStat == 2)
					{
						EV_SET(&event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, &clientsMap[client_socket]);
						kevent(kq, &event, 1, NULL, 0, NULL);
						EV_SET(&event, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
						kevent(kq, &event, 1, NULL, 0, NULL);
					}
				}
			}
			else if (events[i].filter == EVFILT_WRITE)
			{
				Client *data = (Client *)events[i].udata;
				int client_socket = events[i].ident;
				std::string responseStr;

				if (data->getReq().getRequestData().first.empty())
					data->getRes() = Response(Response::GetMimeType(data->getReq().getHeaderData().url),
												data->getReq().getHeaderData().url,
												data->getReq().getHeaderData().requestMethod,
												data->getReq().getHeaderData().url,
												data->getReq().getRequestData().codeStatus,
												data->getReq().getRequestData().isDirListening,
												data->getReq().getHeaderData().url,
												data->getReq().getRequestData().redirection,
												data->getReq().getRequestData().default_page,
												data->getReq().getRequestData().isUpload,
												data->getReq().getRequestData().isCgi,
												data->getReq().getConfigFileObj().server,
												data->getReq().getHeaderData().urlFinal,
												data->getReq().getRequestData().timeOut,
												data->getReq().getRequestData().cgiError,
												data->getReq().getRequestData().codeStatusMap);
				data->getReq().getRequestData().first = "not empty";

				data->getRes().Res_get_chunk(data->getReq().getRequestData().sent_head);
				responseStr = data->getRes().responseStream.str();
				if (responseStr.empty() && !data->getRes().end)
						continue;
				size_t bytes_sent = send(client_socket, responseStr.data(), responseStr.length(), 0);
				if (bytes_sent <= 0 || bytes_sent == SIZE_MAX)
				{
					data->getReq().getRequestData().first = "";
					data->getRes().bytesRead = 0;
					data->getReq().getRequestData().sent_head = 0;
					std::cerr << "Send error"<< std::endl;
					std::cerr << "send failed" << std::endl;
					close(data->getReq().getRequestData().fd);
					data->getReq().getRequestData().codeStatus = 200;
					EV_SET(&event, data->getReq().getRequestData().fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
					kevent(kq, &event, 1, NULL, 0, NULL);

					data->getRes().file.close();

					if (data->getRes().unlink_cgi)
					{
						std::remove(clientsMap[client_socket].getCgi().cgi_output.c_str());
						std::remove(clientsMap[client_socket].getCgi().cgi_error.c_str());
					}

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
						data->getReq().getRequestData().bytes_sent += bytes_sent;
						if ((size_t)data->getRes().bytesRead >= data->getRes().Res_Size || data->getRes().end)
						{
							data->getReq().getRequestData().first = "";
							data->getRes().bytesRead = 0;
							data->getReq().getRequestData().sent_head = 0;

							data->getRes().file.close();
							data->getReq().getRequestData().codeStatus = 200;
							if (data->getRes().unlink_cgi)
							{
								std::remove(clientsMap[client_socket].getCgi().cgi_output.c_str());
								std::remove(clientsMap[client_socket].getCgi().cgi_error.c_str());
							}
							if (data->getReq().getHeaderData().isAlive == 0)
				 			{
								EV_SET(&event, data->getReq().getRequestData().fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
								kevent(kq, &event, 1, NULL, 0, NULL);
								close(client_socket);
								data->getReq().clearData();
							}
							else
							{
								EV_SET(&event, data->getReq().getRequestData().fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
								kevent(kq, &event, 1, NULL, 0, NULL);
								EV_SET(&event, data->getReq().getRequestData().fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
								kevent(kq, &event, 1, NULL, 0, NULL);
								data->getReq().clearData();
							}
						}
				}
			}
			else if (events[i].filter == EVFILT_PROC && (((Client*)events[i].udata)->getReq().getRequestData().timeOut.empty()))
			{
				pid_t pid = (pid_t)events[i].ident;
				Client* data = (Client*)events[i].udata;
				data->getCgi().handleProcessExit(pid, data->getReq().getRequestData().fd, kq, data);
			}
			else if (events[i].filter == EVFILT_TIMER)
			{
				Client* data = (Client*)events[i].udata;
				data->getCgi().handleTimeout(event.ident, data->getReq().getRequestData().fd, kq, data);
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
	int opt = 1;
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
		close(server_fd);
		exit(1);
	}

	int sendbuf_size = 256 * 1024;
	setsockopt(server_fd, SOL_SOCKET, SO_SNDBUF, &sendbuf_size, sizeof(sendbuf_size));

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(this->host.c_str());
	server_addr.sin_port = htons(this->port);

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
