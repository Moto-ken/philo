# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/16 13:41:09 by kemotoha          #+#    #+#              #
#    Updated: 2026/02/16 14:13:14 by kemotoha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME        := philo
CC          := cc
CFLAGS      := -Wall -Wextra -Werror

SRCS        := philo.c \
               print.c \
			   rules_create.c \
			   philosopher_create.c \
			   run_thread.c \
               run_monitor.c \
               run_philos.c \
               free.c \
			   libft.c \
			   utile.c

OBJS        := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
