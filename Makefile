# Executable name
NAME = pipex

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

# Source files
SRCS =	pipex.c \
		pipex_utils.c \
		error_handler.c

# Object files
OBJS = $(SRCS:.c=.o)

# Libft
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# .h files
INCLUDES = -I. -I$(LIBFT_DIR)

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

# Compile .c files into .o files
%.o: %.c pipex.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	$(RM) $(OBJS)

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
