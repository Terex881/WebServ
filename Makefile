NAME = webserv

CC = c++

FLAG = -g -fsanitize=address

OBJ = 	static/main.o  static/Server.o static/File_Parsing.o \
		Boundary.o Chunked.o Request.o Header.o ChunkedBoundary.o\
		Body.o

HEADERS = static/Server.hpp static/File_Parsing.hpp static/DynamicStruct.hpp  Request.hpp Body.hpp Header.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm *.py Zip/*.pdf Zip/*jpg Zip/*.jpeg Zip/*.png Zip/*.mp4 Zip/*txt .DS_Store Zip/ok. *jpg *png

re: fclean all