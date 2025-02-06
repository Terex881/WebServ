NAME = webserv

CC = c++

FLAG = -Wall -Wextra -Werror -std=c++98

OBJ =	main.o	Method/Delete.o	Server/Server.o	Config/File_Parsing.o	cgi-bin/Cgi.o	Response.o Server/Client.o\
		Method/Request/Boundary.o	Method/Request/Chunked.o	Method/Request/Request.o	Method/Request/Header.o	Method/Request/ChunkedBoundary.o	Method/Request/Body.o

HEADERS =	Method/Delete.hpp	Config/File_Parsing.hpp	Config/DynamicStruct.hpp	cgi-bin/Cgi.hpp Response.hpp\
			Method/Request/Request.hpp Server/Server.hpp Server/Client.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all