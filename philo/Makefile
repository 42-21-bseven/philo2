CC		=		gcc

DIR		=		./srcs/

HEADER	=		./includes/philo.h

NAME	=		philo

RM		=		rm -f

SRCS	=		$(DIR)philo.c \
				$(DIR)check_and_parse.c \
				$(DIR)init.c \
				$(DIR)support_func.c

OBJS	=		$(SRCS:.c=.o)

.c.o:
				$(CC) -c $< -o $(<:.c=.o)

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) -Wall -Werror -Wextra -g $(SRCS) -o $(NAME)

%.o:			%c $(HEADER)
				@gcc -c $<

clean:
				$(RM) $(OBJS)

fclean:			clean
					$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re