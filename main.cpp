#include "Server.hpp"

#include "Config/File_Parsing.hpp"



int main(int argc, char **argv)
{
	// try{
		signal(SIGPIPE, SIG_IGN);
	///////////////////////////////////////
		if (argc != 2)
			return 0;
		char *end = NULL;
		File_Parsing p(argv[1]);

		int size = p.servers_count;
		Server* server[size];
		int fd[size];
		int i = 0;

		// Initialize servers
		for (vector<dt>::iterator it = p.host_port.begin(); it != p.host_port.end(); it++)
		{
			server[i++] = new Server(it->key, std::strtod(it->val.c_str(), &end));
			if (*end)
				return 1;
		}
		i = 0;
		while (i < size)
		{
			// Initialize each server and get their listening socket fd
			fd[i] = server[i]->ft_server_init();
			i++;
		}
		i =0;
		while(i < size)
		{
			server[i]->ft_start(size, fd);
		}
	// }
	// catch(const std::exception &e)
	// {
	//     std::cout << e.what() << std::endl;
	// }



	return 0;
}