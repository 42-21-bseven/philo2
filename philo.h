//
// Created by Brooks Seven on 10/15/21.
//

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define LLU long long unsigned

typedef struct	s_philos
{
	int					name_philo;
	unsigned 			left_fork;
	unsigned 			right_fork;
	struct s_args 		*args;
	LLU					last_eat;
	pthread_t 			ph_thread;
	int 				full_saturation;

}								t_philos;

typedef struct	s_args
{
	int					number_of_philosophers;
	int 			    time_to_die;
	int 			    time_to_eat;
	int 			    time_to_sleep;
	int  				numb_of_meals;
	pthread_mutex_t 	*forks;
	LLU					born_time;
	t_philos 			*philos;
	pthread_t 			dead_thread;
	pthread_mutex_t 	output;

}								t_args;

#endif //PHILO2_PHILO_H

