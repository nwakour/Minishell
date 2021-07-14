# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmahjour <hmahjour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/29 19:06:53 by nwakour           #+#    #+#              #
#    Updated: 2021/07/14 17:10:45 by hmahjour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: 		all fclean clean re

NAME			=	minishell

SRCS			=	minishell.c builtins.c env.c fd_files.c handle_quotes.c make_ref.c parse.c helpers.c cmd.c history.c terminal.c path.c system.c readline.c

OBJS			=	${SRCS:.c=.o}

CC				=	gcc

FLAGS           =	-Wall -Wextra -Werror -ltermcap

READLINE		= -lreadline -L /Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include

RM				=	rm -f

INCL 			=	-I includes -L includes 

LIBFLAGS 		= 	-I ./libft -L ./libft -L . ./libft/*.c 

LIBFT_PATH 		= 	./libft

all:			$(NAME) libft_all

$(NAME):	$(OBJS)
				@$(CC) -o $(NAME) $(SRCS) $(INCLUDE) $(LIBFLAGS) $(FLAGS) $(READLINE)

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
