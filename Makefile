NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I.

# dossiers
LIBFT_DIR 	=	libft/
SRCS_DIR	=	srcs/
OBJ_DIR		=	obj/

SRCS =  init.c \
	main.c \
	split.c \

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

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re