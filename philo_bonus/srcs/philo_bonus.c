/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bseven <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 11:08:49 by bseven            #+#    #+#             */
/*   Updated: 2021/10/31 11:08:51 by bseven           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Brooks Seven on 10/15/21.
//

#include "../includes/philo_bonus.h"

void	ft_eat(t_philos *philo)
{
	t_args	*args;

	args = (t_args *)philo->args;
	sem_wait(args->forks);
	ft_output(philo, "take fork in LEFT hand");
	sem_wait(args->forks);
	ft_output(philo, "take fork in RIGHT hand");
	ft_output(philo, "is OM NOM NOM doing");
	ft_usleep(args->time_to_eat);
	sem_post(args->forks);
	ft_output(philo, "PUT the fork of left hand");
	sem_post(args->forks);
	ft_output(philo, "PUT the fork of right hand");
	philo->last_eat = current_time();
	if (philo->full_saturation < args->numb_of_meals)
		philo->full_saturation++;
	if (philo->full_saturation == args->numb_of_meals)
		exit(0);
}

int	*uber_dead_thread(t_philos *dest)
{
	while (1)
	{
		if (dest->args->time_to_die
			< (long long)(current_time() - dest->last_eat))
		{
			sem_wait(dest->args->output);
			printf("%lli philo#%d is dead\n",
				current_time() - dest->args->born_time,
				dest->name_philo);
			ft_usleep(4);
			exit(0);
		}
		if (dest->full_saturation == dest->args->numb_of_meals)
			exit (0);
	}
}

void	*philos_thread(void *src)
{
	t_philos	*philo;

	philo = (t_philos *)src;
	while (1)
	{
		ft_eat(philo);
		ft_output(philo, "is sleeping.");
		ft_usleep(philo->args->time_to_sleep);
		ft_output(philo, "is thinking.");
	}
}

int	create_threads(t_args *args)
{
	int	i;

	i = 0;
	args->born_time = current_time();
	while (i < args->number_of_philosophers)
	{
		args->pid[i] = fork();
		if (args->pid[i] < 0)
			return (1);
		else if (args->pid[i] == 0)
		{
			if (pthread_create(&args->philos[i].ph_thread, NULL,
					philos_thread, &args->philos[i]) != 0)
				return (put_err(" thread create."));
			uber_dead_thread(&(args->philos[i++]));
		}
		i++;
	}
	waitpid(-1, NULL, 0);
	return (0);
}

int	main(int ac, char **av)
{
	t_args	args;

	if (check_args(ac, av) < 0)
		return (put_err(" in args!"));
	parse_args(&args, av);
	init_null(&args);
	if (check_vol(args))
		return (1);
	if (init_table(&args))
		return (1);
	init_philo(&args);
	if (create_threads(&args))
		return (1);
	all_free(&args);
	return (0);
}
