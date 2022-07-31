NAME =		philo
CC =		gcc
FLAGS =		-Wall -Werror -Wextra -pthread
RM =		rm -rf

SRC =		philo_bonus.c \
			dinner_bonus.c \
			utils_bonus.c

OBJECTS =	$(SRC:.c=.o)

.c.o:
			$(CC) $(FLAGS) -c $< -o $(<:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJECTS) philo_bonus.h
			$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)

clean:
			$(RM) $(OBJECTS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
