# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/03 16:19:01 by jmehlig           #+#    #+#              #
#    Updated: 2022/06/23 10:43:08 by jmehlig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_PATH = srcs/

SRCS =	minishell.c \
		lexer1.c \
		lexer2.c \
		parser1.c \
		parser2.c \
		parser3.c \
		parser4.c \
		parser5.c \
		parser6.c \
		parser7.c \
		parser8.c \
		parser9.c \
		parser10.c \
		parser11.c \
		parser12.c \
		parser13.c \
		parser14.c \
		parser15.c \
		parser16.c \
		executor1.c \
		executor2.c \
		executor3.c \
		executor4.c \
		executor5.c \
		executor6.c \
		executor7.c \
		executor8.c \
		executor9.c \
		executor10.c \
		built_ins1.c \
		ast_node.c \
		echo.c \
		ft_split_quotes1.c \
		ft_split_quotes2.c \
		expander1.c \
		expander2.c \
		my_cd.c \
		built_ins2.c \
		list_utils1.c \
		list_utils2.c \
		list_utils3.c \
		check_path.c \
		convert_to_canonical_form1.c \
		convert_to_canonical_form2.c \
		convert_to_canonical_form3.c \
		built_ins3.c \
		signals.c \
		error1.c \
		error2.c \
		my_unset.c \

LIBRARIES = -lreadline -lncurses -L /goinfre/$(USER)/.brew/opt/readline/lib
INCLUDES = -I /goinfre/$(USER)/.brew/opt/readline/include
LIBFT = ${LIBFT_PATH}libft.a

OBJS_PATH = objs
OBJS = $(patsubst %.c, %.o, $(SRCS))

#rm -rf $HOME/.brew && rm -rf $HOME/goinfre/.brew && git clone --depth=1 https://github.com/Homebrew/brew $HOME/goinfre/.brew && echo 'export PATH=$HOME/goinfre/.brew/bin:$PATH' >> $HOME/.zshrc && source $HOME/.zshrc && brew update

#LIBRARIES	= -lreadline -L/Users/$(USER)/.brew/opt/readline/lib -lft -Lincludes/libft/
#INCLUDES	= -I/Users/$(USER)/.brew/opt/readline/include

LIBFT_PATH	= ./includes/libft/

CC		=		gcc

CFLAGS	=		-Wall -Wextra -Werror

all:	$(NAME)

${LIBFT}:
	@make -C $(LIBFT_PATH)

$(NAME): ${LIBFT} $(addprefix $(SRCS_PATH), $(SRCS))
	$(CC) $(CFLAGS) $(INCLUDES) -c $(addprefix $(SRCS_PATH), $(SRCS))
	@mkdir -p $(OBJS_PATH)
	@mv $(OBJS) $(OBJS_PATH)
	$(CC) $(CFLAGS) $(LIBRARIES) $(INCLUDES) $(addprefix $(OBJS_PATH)/, $(OBJS)) ${LIBFT_PATH}libft.a -o $(NAME)

clean:
			make clean -C $(LIBFT_PATH)
			rm -rf $(OBJS_PATH)

fclean_libft:
			make fclean -C $(LIBFT_PATH)

fclean:		clean fclean_libft
			$(RM) $(NAME)

re:			fclean
			$(MAKE)

.PHONY:	target_libft
