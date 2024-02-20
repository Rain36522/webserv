

FLAGS = -pedantic -std=c++98 -Wall -Wextra -Werror -fsanitize=address
NAME = webserv
CC = c++

MAIN = src/main.cpp
# GETIMG = getImg.cpp
SRC1 = Route.cpp Server.cpp WebServer.cpp ParseConfig.cpp Request.cpp Response.cpp sendPage.cpp

SRC1 += $(addprefix imgManager/, $(GETIMG))
SRC1 += $(addprefix request/, $(REQUEST))

SRC = $(addprefix src/, $(SRC1))
SRC += $(MAIN)

OBJ = $(SRC:.cpp=.o)

all : $(NAME)
	./$(NAME) config.yml

$(NAME): $(SRC)
	$(CC) $(SRC) $(FLAGS) -o $(NAME)

clean :

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re


