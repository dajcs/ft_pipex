# Executable name
NAME = pipex
NAME_BONUS = pipex

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

# Source files
SRCS = pipex.c \
       pipex_utils.c \
       error_handler.c

SRCS_BONUS = pipex_bonus.c \
             pipex_utils_bonus.c \
             error_handler_bonus.c \
             here_doc_bonus.c

# Object files
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

# Libft
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# .h files
INCLUDES = -I. -I$(LIBFT_DIR)

# Rules
all: $(NAME)

# Mandatory part rule
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

# Bonus part rule
bonus: $(OBJS_BONUS) $(LIBFT)
	$(CC) $(CLFAGS) $(OBJS_BONUS) $(LIBFT) -o $(NAME_BONUS)

$(LIBFT):
	@make -C $(LIBFT_DIR)

# Compile .c files into .o files
%.o: %.c pipex.h pipex_bonus.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	$(RM) $(OBJS) $(OBJS_BONUS)

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
