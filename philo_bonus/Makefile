CC		=		gcc

DIR		=		./srcs/

HEADER	=		./includes/philo_bonus.h

NAME	=		philo_bonus

RM		=		rm -f

SRCS	=		$(DIR)philo_bonus.c \
				$(DIR)chk_and_parse.c \
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