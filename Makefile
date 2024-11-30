CC = c++
CC += -Wall -Wextra -Werror -g -fsanitize=address
RM = rm -f
NAME = webserv

SRC_FILE = main.cpp Delete.cpp Server.cpp
SRC_HEADER = Delete.hpp Server.hpp

all: $(NAME) 

OBJS = $(SRC_FILE:.cpp=.o)

$(NAME): $(OBJS)
	$(CC) $(SRC_FILE) -o $(NAME)

%.o: %.cpp $(SRC_HEADER)
	$(CC) -c $< -o $@

clean:
	$(RM) -rf $(OBJS)

fclean: clean
	$(RM) -rf webserv
re: clean all