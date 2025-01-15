NAME = webserv

CC = c++

# FLAG = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g        

OBJ =	main.o	Method/Delete.o	Server.o	Config/File_Parsing.o	cgi-bin/Cgi.o	Response.o\
		Method/Request/Boundary.o	Method/Request/Chunked.o	Method/Request/Request.o	Method/Request/Header.o	Method/Request/ChunkedBoundary.o	Method/Request/Body.o

HEADERS =	Method/Delete.hpp	Server.hpp	Config/File_Parsing.hpp	Config/DynamicStruct.hpp	cgi-bin/Cgi.hpp Response.hpp\
			Method/Request/Request.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm *.py /Users/sdemnati/goinfre/UP/* .DS_Store

re: fclean all