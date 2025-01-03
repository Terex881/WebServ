NAME = webserv

CC = c++

FLAG = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

        

OBJ =	main.o	Delete.o	Server.o	File_Parsing.o	Cgi.o	Response.o\
		Request/Boundary.o	Request/Chunked.o	Request/Request.o	Request/Header.o	Request/ChunkedBoundary.o	Request/Body.o

HEADERS =	Delete.hpp	Server.hpp	File_Parsing.hpp	DynamicStruct.hpp	Cgi.hpp\
			Request/Request.hpp	Request/Header.hpp	Request/Body.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm  *.py *.pdf *jpg *.png *.mp4 Zip/*.py Zip/*.pdf Zip/*jpg *.png Zip/*.mp4

re: fclean all