

FLAGS = -pedantic -std=c++98 -Wall -Wextra -Werror
NAME = webserv
CC = c++

MAIN = test3.cpp
GNL = get_next_line.c get_next_line_utils.c
GETIMG = getImg.cpp
REQUEST = getPage.cpp request.cpp
SRC1 = Route.cpp Server.cpp WebServer.cpp

SRC1 += $(addprefix gnl/, $(GNL))
SRC1 += $(addprefix imgManager/, $(GETIMG))
SRC1 += $(addprefix request/, $(REQUEST))

SRC = $(addprefix src/, $(SRC1))
SRC += $(MAIN)

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

