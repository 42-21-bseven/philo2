/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bseven <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 11:07:23 by bseven            #+#    #+#             */
/*   Updated: 2021/10/31 11:07:44 by bseven           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Brooks Seven on 10/15/21.
//

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

# define LLU long long unsigned

typedef struct s_philos
{
	int					name_philo;
	struct s_args		*args;
	LLU					last_eat;
	pthread_t			ph_thread;
	int					full_saturation;
}								t_philos;

typedef struct s_args
{
	int					number_of_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					numb_of_meals;
	sem_t				*forks;
	LLU					born_time;
	t_philos			*philos;
	sem_t				*output;
	pid_t				*pid;
}								t_args;

int		ft_atoi(char *str);
int		chk_not_num(char *str);
int		check_args(int ac, char **av);
void	parse_args(t_args *args, char **av);
int		check_vol(t_args args);
void	init_null(t_args *args);
int		init_table(t_args *args);
void	init_philo(t_args *args);
int		put_err(char *str);
LLU		current_time(void);
void	ft_usleep(int time);
void	ft_output(t_philos *philos, char *str);
void	all_free(t_args *args);
int		create_threads(t_args *args);
void	*philos_thread(void *src);
int		*uber_dead_thread(t_philos *dest);
void	ft_eat(t_philos *philo);

#endif //PHILO2_PHILO_H
