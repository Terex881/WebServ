NAME = webserv

CC = c++

# FLAG = -Wall -Wextra -Werror -std=c++98

OBJ = 	main.o Delete.o server.o File_Parsing.o  Cgi.o Response.o\
		RequestParser/parseBoundry.o RequestParser/parseChunked.o RequestParser/Request.o\
		RequestParser/parseHeader.o

HEADERS = Delete.hpp Server.hpp File_Parsing.hpp DynamicStruct.hpp Cgi.hpp RequestParser/Request.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

run: re clean
	./$(NAME) file.conf

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm file_to_read.txt *.py *.pdf *jpg *.png *.mp4 *txt

re: fclean all