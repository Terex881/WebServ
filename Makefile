NAME = webserv

CC = c++

FLAG = -Wall -Wextra -Werror -std=c++98

        

OBJ = main.o Delete.o server.o File_Parsing.o Request.o Cgi.o Response.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp Request.hpp Delete.hpp Server.hpp File_Parsing.hpp DynamicStruct.hpp Cgi.hpp
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm file_to_read.txt *.py *.pdf *jpg *.png *.mp4

re: fclean all