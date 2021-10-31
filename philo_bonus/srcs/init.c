/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bseven <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 11:08:38 by bseven            #+#    #+#             */
/*   Updated: 2021/10/31 11:08:42 by bseven           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	init_null(t_args *args)
{
	args->forks = NULL;
	args->philos = NULL;
	args->output = NULL;
	args->pid = NULL;
}

int	init_table(t_args *args)
{
	args->philos = malloc(sizeof(t_philos) * args->number_of_philosophers);
	if (!args->philos)
		return (put_err(" philos alloc."));
	sem_unlink("forks");
	sem_unlink("output");
	args->output = sem_open("output", O_CREAT, 0666, 1);
	args->forks = sem_open("forks", O_CREAT, 0666,
			   args->number_of_philosophers);
	args->pid = malloc(sizeof (pid_t) * args->number_of_philosophers);
	if (!args->pid)
		return (put_err(" forks alloc."));
	return (0);
}

void	init_philo(t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_philosophers - 1)
	{
		args->philos[i].name_philo = i + 1;
		args->philos[i].args = args;
		args->philos[i].last_eat = current_time();
		args->philos[i].full_saturation = 0;
		i++;
	}
	args->philos[i].name_philo = i + 1;
	args->philos[i].args = args;
	args->born_time = current_time();
	args->philos[i].last_eat = current_time();
	args->philos[i].full_saturation = 0;
}
