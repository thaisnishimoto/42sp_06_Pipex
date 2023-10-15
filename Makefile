# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/01 12:48:28 by tmina-ni          #+#    #+#              #
#    Updated: 2023/10/15 16:28:34 by tmina-ni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#================================FLAGS=========================================#

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LIBFT_FLAG = -L$(LIB_PATH) -lft
MAKE_NO_PRINT = $(MAKE) --no-print-directory

#================================PATHS=========================================#

LIB_PATH = ./libft/
HEADER_PATH = ./include
SRC_PATH = ./src
BONUS_PATH = ./src_bonus

#================================FILES=========================================#

NAME = pipex
BONUS_NAME = pipex_bonus
SRC = $(wildcard $(SRC_PATH)/*.c)
OBJ = $(SRC:.c=.o)
SRC_BONUS = $(wildcard $(BONUS_PATH)/*.c)
OBJ_BONUS = $(SRC_BONUS:.c=.o)
HEADER = $(HEADER_PATH)/pipex.h
HEADER_BONUS = $(HEADER_PATH)/pipex_bonus.h

#================================RULES=========================================#

all: libft $(NAME)

libft:
	$(MAKE_NO_PRINT) -C $(LIB_PATH)

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_FLAG)

$(SRC_PATH)/%.o: $(SRC_PATH)/%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: libft $(BONUS_NAME)

$(BONUS_NAME): $(OBJ_BONUS) $(HEADER_BONUS)
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $(OBJ_BONUS) $(LIBFT_FLAG)

$(BONUS_PATH)/%.o: $(BONUS_PATH)/%.c $(HEADER_BONUS)
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
	rm -f $(OBJ) $(OBJ_BONUS)
	$(MAKE_NO_PRINT) clean -C $(LIB_PATH)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)
	$(MAKE_NO_PRINT) fclean -C $(LIB_PATH)

re: fclean all

rebonus: fclean bonus

.PHONY: all libft bonus run leak gdb clean fclean re rebonus
