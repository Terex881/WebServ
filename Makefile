NAME = request

CC = c++

FLAG =  -g -fsanitize=address -std=c++98

OBJ = main.o Request.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAG) -o $@

%.o: %.cpp Request.hpp
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) rm file_to_read.txt resume.py profile.jpg file*

re: fclean all