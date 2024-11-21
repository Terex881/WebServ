#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

int main()
{
    // Step 1: Create the socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cout << "socket failed" << std::endl;
        exit(1);
    }

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
    if (listen(server_fd, 1000) == -1)
    {
        std::cout << "listen failed" << std::endl;
        exit(1);
    }

    // Step 4: Accept and handle client connections
    while (1)
    {
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &addrlen);

        if (new_socket < 0)
        {
            std::cout << "accept failed" << std::endl;
            continue;  // Skip to the next iteration if accept fails
        }

        // Step 5: Receive data from the client
        char buffer[1024] = {0};
        int bytes_received = recv(new_socket, buffer, sizeof(buffer), 0);
        if (bytes_received < 0)
        {
            std::cout << "recv failed" << std::endl;
        }
        else
        {
            std::cout << "Received request:\n" << buffer << std::endl;
        }

        // Step 6: Prepare HTTP response
        std::string message = "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: " + std::to_string(11) + "\r\n"
                            "\r\n" +  // Blank line separating headers and body
                            "Hello world";

        // Step 7: Send HTTP response to client
        int bytes_sent = send(new_socket, message.c_str(), message.length(), 0);
        if (bytes_sent < 0)
        {
            std::cout << "send failed" << std::endl;
        }

        // Step 8: Close the client socket after the response
        close(new_socket);
    }

    // Never reached, but if we were to stop the server, we'd close the server socket.
    close(server_fd);

    return 0;
}
