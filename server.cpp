#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

// step1 :      creation of mini server that accept any kind of request using socket  creates a socket using a specific communication domain (e.g., AF_INET for IPv4) and a socket type (e.g., SOCK_STREAM for TCP, SOCK_DGRAM for UDP).
//and it return a fail descriptor of that socket AF_INET: Address family (IPv4).
//SOCK_STREAM: Specifies that we are using a TCP connection (reliable, stream-based).
//0: Specifies the protocol (0 selects the default protocol for the given socket type, which is TCP for SOCK_STREAM).



//step 2:       binding the socket , The bind function is used to bind a socket to a particular IP address and port, allowing it to receive data at that address and port.


//step 3:       After binding the socket, the server needs to start listening for incoming connection requests. This is where the server gets ready to accept connections from clients


//step 4:       accept() extracts the first connection request on the queue of pending connections, creates a new socket for the communication with the client, and returns a new socket file descriptor.

//step 5: Once the connection is established, the server can start sending and receiving data with the client. This is done using functions like send(), recv(), read(), and write(). These allow the server to exchange data with the client.




int main()
{
	//step 1
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		std::cout<< "socket failed" << std::endl;
		exit(1);
	}
	struct sockaddr_in server_addr;
	socklen_t server_addr_len = sizeof(server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(8080);
	if (bind(server_fd, (struct sockaddr*)&server_addr, server_addr_len) == -1)
	{
		std::cout<< "bind failed" << std::endl;
		exit(1);
	}
	while (1)
	{
		//step 2
		std::cout << "Server is listening on localhost:8080...\n";
		//step 3
		if (listen(server_fd, 1000) == -1)
		{
			std::cout<< "listen failed" << std::endl;
			exit(1);	
		}
		//step 4
		struct sockaddr_in client_address;
		socklen_t addrlen = sizeof(client_address);
		int new_socket = accept(server_fd, (struct sockaddr*)&client_address, &addrlen);

		if (new_socket < 0)
		{
			std::cout<< "accept failed" << std::endl;
			exit(1);
		}

		char buffer[1024] = {0};
		recv(new_socket, buffer, sizeof(buffer), 0); 

		std::string message = "HTTP/1.1 200 OK\r\n"
							"Content-Type: text/plain\r\n"
							"Content-Length: " + std::to_string(10) + "\r\n"
							"\r\n" +  // Blank line separating headers and body
							"hell world";
		send(new_socket, message.c_str(), strlen(message.c_str()), 0);
	}
}