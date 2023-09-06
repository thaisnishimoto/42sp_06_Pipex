#**************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/28 20:22:45 by tmina-ni          #+#    #+#              #
#    Updated: 2023/06/10 16:29:34 by tmina-ni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = program
CC = cc
CFLAGS = -Wall -Wextra -Werror 
SRC = ../get_next_line.c ../get_next_line_utils.c
BONUS = $(wildcard ../*bonus.c)

# Default BUFFER_SIZE value
BUFFER_SIZE ?= 1

all:
	$(CC) -g3 $(CFLAGS)-D BUFFER_SIZE=$(BUFFER_SIZE) test_gnl.c $(SRC) -lrt -lm -o $(NAME)
	./program

bonus:
	$(CC) -g3 $(CFLAGS)-D BUFFER_SIZE=$(BUFFER_SIZE) test_gnl.c $(BONUS) -lrt -lm -o $(NAME)
	./program

clean:
	rm -f *.o

fclean: clean
	rm -f program
