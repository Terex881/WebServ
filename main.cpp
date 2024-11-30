#include "Server.hpp"
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_CLIENTS 128

int main()
{
    int size = 2;
    Server* server[size];
    int fd[size];

    // Initialize servers
    server[0] = new Server("127.0.0.1", 6000);
    server[1] = new Server("127.0.0.1", 8080);
    
    // Initialize each server and get their listening socket fd
    fd[0] = server[0]->ft_server_init();
    fd[1] = server[1]->ft_server_init();

    //start servers
    server[0]->ft_start(size, fd);

    return 0;
}
