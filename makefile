# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/29 19:06:53 by nwakour           #+#    #+#              #
#    Updated: 2021/10/29 17:47:49 by hmahjour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: 		all fclean clean re

NAME			=	minishell

SRCS			=	builtins.c builtins_most.c builtins_more.c builtind_more_than_more.c \
					cmd.c cmd_more.c env.c fd_files.c ft_exit.c handle_quotes.c\
					helpers.c helpers_more.c make_ref.c minishell.c parse_more.c parse.c\
					path.c readline.c system_more_than_more.c system_more.c system.c handle_var.c

OBJS			=	${SRCS:.c=.o}

CC				=	gcc

FLAGS           =	-Wall -Wextra -Werror -ltermcap -lreadline

READLINE		=  -L/Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include

RM				=	rm -f

INCL 			=	-I includes -L includes 

LIBFLAGS 		= 	-I ./libft -L ./libft -L . ./libft/*.c 

LIBFT_PATH 		= 	./libft

all:			libft_all $(NAME)

$(NAME):	$(SRCS)
				@$(CC) $(FLAGS) -o $(NAME) $(SRCS) $(INCLUDE) $(LIBFLAGS)  $(READLINE)
				@echo "Minishell was created successfully"

clean:			libft_clean
				@$(RM) $(OBJS)

fclean:			libft_fclean clean
				@$(RM) $(NAME)

re:				fclean all

libft_all:
				@make -C $(LIBFT_PATH) all
				@cp ./libft/libft.a libft.a

libft_clean:
				@make -C $(LIBFT_PATH) clean

libft_fclean:
				@make -C $(LIBFT_PATH) fclean
				@$(RM) libft.a
