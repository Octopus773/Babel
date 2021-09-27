##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## test
##

CC	=	g++

SRC 	=	common/Audio.cpp	\

MAIN	=	common/main.cpp	\

OBJ	=	$(SRC:%.cpp=%.o)

MAINO	=	$(MAIN:%.cpp=%.o)

INCLUDE = 	-I common/

CFLAGS = -Wall -Wextra -Wshadow

LDFLAGS = -lpthread -lportaudio

NAME = audio

all: $(NAME)

$(NAME): $(MAINO) $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(MAINO) $(LDFLAGS) $(INCLUDE) $(CFLAGS)

clean:
	rm -f $(MAINO) $(OBJ)
	rm -f *~
	rm -f common/*~

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
