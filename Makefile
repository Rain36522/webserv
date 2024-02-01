

FLAGS = -pedantic -std=c++98 -Wall -Wextra -Werror
NAME = webserv
CC = c++

MAIN = test3.cpp
GETIMG = getImg.cpp downloadGPT.cpp put_in_file.cpp
REQUEST = getPage.cpp request.cpp
#SRC1 = Route.cpp Server.cpp WebServer.cpp

SRC1 = $(addprefix imgManager/, $(GETIMG))
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

