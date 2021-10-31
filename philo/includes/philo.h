/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bseven <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 13:00:37 by bseven            #+#    #+#             */
/*   Updated: 2021/10/31 13:00:39 by bseven           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define LLU long long unsigned

typedef struct s_philos
{
	int					name_philo;
	unsigned int		left_fork;
	unsigned int		right_fork;
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
	pthread_mutex_t		*forks;
	LLU					born_time;
	t_philos			*philos;
	pthread_t			dead_thread;
	pthread_mutex_t		output;
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

#endif //PHILO2_PHILO_H
