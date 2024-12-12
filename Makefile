
MSRCS = Cgi.cpp	Delete.cpp	File_Parsing.cpp Request.cpp	main.cpp	server.cpp
MOBJS = $(MSRCS:.cpp=.o)

HEADER = Cgi.hpp	DynamicStruct.hpp	Request.hpp	Delete.hpp	File_Parsing.hpp	Server.hpp

CFLAGS =  -std=c++98 -Wall -Wextra -Werror

NAME =  webserv

all : $(NAME)

%.o:%.cpp $(HEADER)
	c++ $(CFLAGS) -c $< -o $@

$(NAME): $(MOBJS)
	c++ $(CFLAGS) $^ -o $(NAME)

clean:
	rm -f $(MOBJS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
