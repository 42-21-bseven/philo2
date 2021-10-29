//
// Created by Brooks Seven on 10/15/21.
//

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

#define LLU long long unsigned

typedef struct	s_philos
{
	int					name_philo;
	struct s_args 		*args;
	LLU					last_eat;
	pthread_t 			ph_thread;
	int 				full_saturation;
	pid_t				*pid;

}								t_philos;

typedef struct	s_args
{
	int					number_of_philosophers;
	int 			    time_to_die;
	int 			    time_to_eat;
	int 			    time_to_sleep;
	int  				numb_of_meals;
	sem_t				*forks;
	LLU					born_time;
	t_philos 			*philos;
	sem_t 				*output;
	int					err;
	pid_t 				*pid;
}								t_args;

#endif //PHILO2_PHILO_H

