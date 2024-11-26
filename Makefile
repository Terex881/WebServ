NAME = request

CC = c++

# FLAG = -Wall -Wextra -Werror -std=c++98

OBJ = main.o Request.o
.SILENT: # rmove 
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp Request.hpp
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all