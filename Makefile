NAME = webserv

CC = c++

# FLAG = -Wall -Wextra -Werror -std=c++98

OBJ = 	static/main.o  static/Server.o static/File_Parsing.o Boundary.o Chunked.o Request.o Header.o ChunkedBoundary.o

HEADERS = static/Server.hpp static/File_Parsing.hpp static/DynamicStruct.hpp  Request.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm *.py *.pdf *jpg *.jpeg *.png *.mp4 *txt

re: fclean all