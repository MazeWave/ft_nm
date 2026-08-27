# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/28 17:11:28 by ldalmass          #+#    #+#              #
#    Updated: 2026/08/27 1515:0505:2020 by ldalmass         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ft_nm
NAME_B	=	ft_nm_bonus

SRC		=	src/main.c \
			src/log.c \
			src/parser_opt.c \

OBJS	=	$(SRC:.c=.o)

CC		=	gcc

UNAME_S	:=	$(shell uname -s)

CFLAGS	=	-Wall -Wextra -Werror -DPRINT_LOGS -std=c23 -D_POSIX_C_SOURCE=200809L -Wextra -Werror -std=c23 -D_POSIX_C_SOURCE=200809L

# Enable logs
CFLAGS += -D_DEFAULT_SOURCE

# Leak check
CFLAGS += -fsanitize=address

# Last git commit
CFLAGS += -DGIT_COMMIT=\"$(shell git rev-parse --short=8 HEAD)\"

# MacOS build
ifeq ($(UNAME_S),Darwin)
	CFLAGS	+=	-D_DARWIN_C_SOURCE
endif

RM		=	rm -rf

#rules
%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
all : $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lm
	@$(RM) $(NAME).o

bonus : all
	@ln -sf $(NAME) $(NAME_B)

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME) $(NAME_B)
	@$(RM)

re : fclean all
