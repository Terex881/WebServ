NAME = webserv

CC = c++

FLAG =  -g -fsanitize=address -std=c++98

OBJ = main.o Delete.o Server.o File_Parsing.o Request.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp Request.hpp Delete.hpp Server.hpp File_Parsing.hpp DynamicStruct.hpp
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm file_to_read.txt *.py

re: fclean all