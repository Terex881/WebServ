NAME = webserv

CC = c++

FLAG = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

        

OBJ = main.o Delete.o Server.o File_Parsing.o  Cgi.o Response.o\
RequestParser/parseBoundry.o RequestParser/parseChunked.o RequestParser/Request.o \
RequestParser/ChunkedBoundary.o RequestParser/Header.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp Delete.hpp Server.hpp File_Parsing.hpp DynamicStruct.hpp Cgi.hpp RequestParser/Request.hpp
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm file_to_read.txt *.py *.pdf *jpg *.png *.mp4

re: fclean all