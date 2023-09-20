# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/01 12:48:28 by tmina-ni          #+#    #+#              #
#    Updated: 2023/09/13 16:20:23 by tmina-ni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#================================FLAGS=========================================#

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LIBFT = -L$(LIB_PATH) -lft
MAKE_NO_PRINT = $(MAKE) --no-print-directory

#================================FILES=========================================#

NAME = pipex
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
HEADER = $(wildcard *.h)
LIB_PATH = ./libft/

#================================RULES=========================================#

all: libft $(NAME)

libft:
	$(MAKE_NO_PRINT) -C $(LIB_PATH)

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	@echo "Arguments: IN, CMD1, CMD2 and OUT"
	./$(NAME) $(IN) $(CMD1) $(CMD2) $(OUT)

#================================LEAKS=========================================#

leak:
	@echo "Arguments: IN, CMD1, CMD2 and OUT"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./$(NAME) $(IN) $(CMD1) $(CMD2) $(OUT)
	
#================================LEAKS=========================================#

gdb:
	@echo "Arguments: IN, CMD1, CMD2 and OUT"
	gdb --tui --args ./$(NAME) $(IN) $(CMD1) $(CMD2) $(OUT)

#================================CLEAN=========================================#

clean:
	rm -f *.o
	$(MAKE_NO_PRINT) clean -C $(LIB_PATH)

fclean: clean
	rm -f $(NAME)
	$(MAKE_NO_PRINT) fclean -C $(LIB_PATH)

re: fclean all

.PHONY: all libft run leak gdb clean fclean re
