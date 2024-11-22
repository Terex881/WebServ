#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>

#define MAX_CLIENTS 128  // Set a limit for the number of clients

// Set a socket to non-blocking mode
void set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main()
{
    // Step 1: Create the server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cout << "socket failed" << std::endl;
        exit(1);
    }

    // Set the server socket to non-blocking
    set_non_blocking(server_fd);

    // Step 2: Setup server address and bind it
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Binding to localhost
    server_addr.sin_port = htons(8080); // Port 8080

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cout << "bind failed" << std::endl;
        exit(1);
    }

    // Step 3: Start listening for connections
    std::cout << "Server is listening on localhost:8080...\n";
    if (listen(server_fd, MAX_CLIENTS) == -1)
    {
        std::cout << "listen failed" << std::endl;
        exit(1);
    }

    // Step 4: Create kqueue
    int kq = kqueue();
    if (kq == -1)
    {
        std::cout << "kqueue creation failed" << std::endl;
        exit(1);
    }

    // Step 5: Register the server socket with kqueue for read events
    struct kevent change;
    EV_SET(&change, server_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1)
    {
        std::cout << "kevent registration for server socket failed" << std::endl;
        exit(1);
    }

    // Step 6: Event loop
    std::vector<int> clients;

    while (1)
    {
        struct kevent events[MAX_CLIENTS];
        int num_events = kevent(kq, NULL, 0, events, MAX_CLIENTS, NULL);

        if (num_events < 0)
        {
            std::cout << "kevent failed" << std::endl;
            exit(1);
        }

        // Step 7: Process events
        for (int i = 0; i < num_events; i++)
        {
            if (events[i].filter == EVFILT_READ)
            {
                if (events[i].ident == server_fd)
                {
                    // Accept a new connection
                    struct sockaddr_in client_addr;
                    socklen_t addrlen = sizeof(client_addr);
                    int new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
                    if (new_socket < 0)
                    {
                        std::cout << "accept failed" << std::endl;
                        continue;
                    }

                    // Set new client socket to non-blocking
                    set_non_blocking(new_socket);

                    // Register the new client socket with kqueue for read events
                    EV_SET(&change, new_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1)
                    {
                        std::cout << "kevent registration for client socket failed" << std::endl;
                        continue;
                    }

                    std::cout << "New client connected: " << new_socket << std::endl;

                    // Add new client socket to the list of active clients
                    clients.push_back(new_socket);
                }
                else
                {
                    // Handle data from an existing client
                    char buffer[1024] = {0};
                    int bytes_received = recv(events[i].ident, buffer, sizeof(buffer), 0);
                    if (bytes_received < 0)
                    {
                        // Handle non-blocking case: no data available
                        if (errno != EWOULDBLOCK && errno != EAGAIN)
                        {
                            std::cout << "recv failed" << std::endl;
                        }
                        continue;
                    }
                    else if (bytes_received == 0)
                    {
                        // Client disconnected, clean up
                        std::cout << "Client disconnected: " << events[i].ident << std::endl;
                        close(events[i].ident);
                        clients.erase(std::remove(clients.begin(), clients.end(), events[i].ident), clients.end());
                    }
                    else
                    {
                        // Print the received request
                        // std::cout << "Received request from client " << events[i].ident << ":\n" << buffer << std::endl;

                        // Step 8: Prepare HTTP response
                        std::string message = "HTTP/1.1 200 OK\r\n"
                                              "Content-Type: text/plain\r\n"
                                              "Content-Length: " + std::to_string(11) + "\r\n"
                                              "\r\n" +  // Blank line separating headers and body
                                              "Hello world";

                        // Send HTTP response to the client
                        int bytes_sent = send(events[i].ident, message.c_str(), message.length(), 0);
                        if (bytes_sent < 0)
                        {
                            std::cout << "send failed" << std::endl;
                        }
                    }
                }
            }
            else if (events[i].filter == EVFILT_EXCEPT)
            {
                // Handle exceptional events like client disconnections
                std::cout << "Exceptional event on client socket: " << events[i].ident << std::endl;
                close(events[i].ident);
                clients.erase(std::remove(clients.begin(), clients.end(), events[i].ident), clients.end());
            }
        }
    }

    // Cleanup: Close server socket and client connections
    close(server_fd);
    for (size_t i = 0; i < clients.size(); ++i)
    {
        close(clients[i]);
    }

    return 0;
}

//93.64 - 2.39