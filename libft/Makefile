# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: camerico <camerico@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 16:21:40 by camerico          #+#    #+#              #
#    Updated: 2025/02/21 20:32:22 by camerico         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

# dossiers
PRINTF 		=	ft_printf/
GNL 		=	gnl/
LIBFT 		=	libft/
INCLUDE     =   includes/

OBJ_DIR		=	obj/

HEADER = ./include/libft.h

LIBFTSRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
    ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
    ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
    ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c \
    ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c \
    ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
    ft_putendl_fd.c ft_putnbr_fd.c ft_split.c ft_atol.c ft_putchar_printf.c ft_putnbr.c \

LIBFTSRCSB = ft_lstnew.c \
      ft_lstsize.c \
      ft_lstlast.c \
      ft_lstadd_back.c \
      ft_lstadd_front.c \
      ft_lstdelone.c \
      ft_lstclear.c \
      ft_lstiter.c \

PRINTFSRCS = ft_itoa.c \
            ft_printf.c \
            ft_putchar.c \
            ft_putstr.c \
            ft_strlen.c \
            print_hexa.c \
            print_integer.c \
            print_unsigned.c \
            print_pointeur.c \

GNLSRCS	= get_next_line.c \
		get_next_line_utils.c \


# LIBFT
SRCS_LIBFT	= 	$(addprefix $(LIBFT), $(LIBFTSRCS))
OBJ_LIBFT	= 	$(addprefix $(OBJ_DIR), $(LIBFTSRCS:.c=.o))
# LIBFT-BONUS
SRCS_LIBFTB	= 	$(addprefix $(LIBFT), $(LIBFTSRCSB))
OBJ_LIBFTB	= 	$(addprefix $(OBJ_DIR), $(LIBFTSRCSB:.c=.o))
# PRINTF
SRCS_PRINTF	=	$(addprefix $(PRINTF), $(PRINTFSRCS))
OBJ_PRINTF	= 	$(addprefix $(OBJ_DIR), $(PRINTFSRCS:.c=.o))
# GET_NEXT_LINE
SRCS_GNL	=	$(addprefix $(GNL), $(GNLSRCS))
OBJ_GNL		= 	$(addprefix $(OBJ_DIR), $(GNLSRCS:.c=.o))


all: $(NAME)


$(NAME): $(OBJ_LIBFT) $(OBJ_LIBFTB) $(OBJ_PRINTF) $(OBJ_GNL)
	@ar rc $(NAME) $(OBJ_LIBFT) $(OBJ_LIBFTB) $(OBJ_PRINTF) $(OBJ_GNL)
	@ranlib $(NAME)
	
$(OBJ_DIR)%.o: libft/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(OBJ_DIR)%.o: ft_printf/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(OBJ_DIR)%.o: gnl/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_LIBFT) $(OBJ_LIBFTB) $(OBJ_PRINTF) $(OBJ_GNL)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus