# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hkalyonc <hkalyonc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/03 16:19:01 by jmehlig           #+#    #+#              #
#    Updated: 2022/05/13 13:45:32 by hkalyonc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_PATH = ./srcs/

GET_NEXT_LINE_PATH = ./includes/get_next_line/

SRCS =	$(SRCS_PATH)minishell.c \
		$(SRCS_PATH)lexer.c \
		$(SRCS_PATH)parser.c \
		$(SRCS_PATH)executor.c \
		$(SRCS_PATH)built_ins1.c \
		$(SRCS_PATH)ast_node.c \
		$(SRCS_PATH)echo.c \
		$(SRCS_PATH)ft_split_quotes.c \

GET_NEXT_LINE_SRCS =	$(GET_NEXT_LINE_PATH)get_next_line.c \
						$(GET_NEXT_LINE_PATH)get_next_line_utils.c \

LIBFT_PATH	= ./includes/libft/

CC		=		gcc

CFLAGS	=		-Wall -Wextra -Werror -D BUFFER_SIZE=1

all:	target_libft $(NAME)

target_libft:
			make -C $(LIBFT_PATH)

$(NAME): target_libft
		$(CC) $(CFLAGS) $(MLXFLAGS) -o $(NAME) $(SRCS) $(GET_NEXT_LINE_SRCS) $(LIBFT_PATH)libft.a

clean:
			make clean -C $(LIBFT_PATH)

fclean_libft:
			make fclean -C $(LIBFT_PATH)

fclean:		clean fclean_libft
			$(RM) $(NAME)

re:			fclean
			$(MAKE)


