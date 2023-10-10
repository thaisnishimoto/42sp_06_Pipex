#*************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/28 20:22:45 by tmina-ni          #+#    #+#              #
#    Updated: 2023/10/10 00:45:28 by tmina-ni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = tester
B_NAME = tester_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 
SRC = tester_pipex.c
BONUS = $(wildcard *bonus.c)
LIBFT = -L../libft -lft

all: pipex $(NAME)
	./$(NAME)
	make --no-print fclean
pipex:
	$(MAKE) -C ../ 

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) -lrt -lm $(LIBFT)

bonus:
	$(MAKE) bonus -C ../ 
	$(CC) $(CFLAGS) -o $(B_NAME) $(BONUS) -lrt -lm $(LIBFT)
	./$(B_NAME)
	make --no-print fclean

clean:
	rm -f *.o
	rm -f ../outfile*

fclean: clean
	rm -f $(NAME) $(B_NAME)

re: fclean all

.PHONY: all pipex bonus clean fclean re
