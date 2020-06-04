# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: pacovali <pacovali@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/06/03 19:50:44 by pacovali       #+#    #+#                 #
#    Updated: 2020/06/04 21:02:01 by pacovali      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME= computor
CPL= g++
FLAGS= -Werror -Wall -Wextra --std=c++1z
SRC= main.cpp computor.class.cpp
OBJ= main.o computor.class.o
HDR= -I .

all: $(NAME)

$(NAME):
	@$(CPL) -c $(FLAGS) $(SRC) $(HDR)
	@$(CPL) -o $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all