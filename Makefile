

FLAGS = -pedantic -std=c++98 -Wall -Wextra -Werror
NAME = webserv
CC = c++

SRC = test.cpp

OBJ = $(SRC:.cpp=.o)

all : $(NAME)
	./$(NAME)

.c.o:
	$(CC) -c -o $@ $< $(FLAGS)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(NAME)

clean :
	rm $(OBJ)

fclean : clean
	rm $(NAME)

re : fclean all

.PHONY : all clean fclean re

