NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -I.

# dossiers
LIBFT_DIR 	=	libft/
SRCS_DIR	=	srcs/
OBJ_DIR		=	obj/

SRCS =  init.c \
	main2.c \
	main3.c \
	main.c \
	split.c \
	split2.c \
	split3.c \
	token.c \
	token2.c \
	expand.c \
	utils.c \
	hd_helper.c \
	commander.c \
	expand_com_init2.c \
	pipes/chpl.c \
	pipes/chpl2.c \
	pipes/chpl3.c \
	pipes/chpl4.c \
	pipes/chpl5.c \
	pipes/list_to_array.c \
	pipes/new_pipe.c \
	pipes/np2.c \
	pipes/np3.c \
	pipes/parent_and_waitpid.c \
	free.c \
	builtin/builtin.c \
	builtin/cd.c \
	builtin/echo.c \
	builtin/exit.c \
	builtin/pwd.c \
	builtin/unset.c \
	builtin/export.c \
	builtin/export2.c \
	builtin/env.c \
	status.c \
	heredoc.c \
	heredoc2.c \
	heredoc3.c \
	redirs.c \
	redirs2.c \
	signal.c \

HEADER = minishell.h

SRCS_MINISHELL 	=	$(addprefix $(SRCS_DIR), $(SRCS))
OBJS_MINISHELL 	= 	$(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

# ajout de la libft
LIBFT = $(LIBFT_DIR)/libft.a

# compilation
all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJS_MINISHELL)
	$(CC) $(CFLAGS) $(OBJS_MINISHELL) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)pipes

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re