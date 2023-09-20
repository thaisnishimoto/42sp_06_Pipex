#*************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/28 20:22:45 by tmina-ni          #+#    #+#              #
#    Updated: 2023/09/12 00:14:46 by tmina-ni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = tester
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 
SRC = $(wildcard *.c)
BONUS = $(wildcard *bonus.c)
LIBFT = -L../libft -lft

all: pipex $(NAME)
	./$(NAME)
	make --no-print clean
pipex:
	$(MAKE) -C ../ 

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) -lrt -lm $(LIBFT)

bonus:
	$(CC) -g3 $(CFLAGS)-D BUFFER_SIZE=$(BUFFER_SIZE) test_gnl.c $(BONUS) -lrt -lm -o $(NAME)
	./program

clean:
	rm -f *.o
	rm -f ../outfile*

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all pipex bonus clean fclean re
