/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bseven <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 11:08:57 by bseven            #+#    #+#             */
/*   Updated: 2021/10/31 11:08:59 by bseven           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	put_err(char *str)
{
	printf("\nERROR%s\n", str);
	return (1);
}

LLU	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(int time)
{
	LLU	t;

	t = current_time();
	while (current_time() - t < (LLU)time)
		usleep(100);
}

void	ft_output(t_philos *philos, char *str)
{
	sem_wait(philos->args->output);
	printf("%llu\tphilo#%d \t...\t %s\n",
		   (current_time() - philos->args->born_time), philos->name_philo, str);
	sem_post(philos->args->output);
}

void	all_free(t_args *args)
{
	int	i;

	i = -1;
	while (++i < args->number_of_philosophers)
		kill(args->pid[i], SIGINT);
	sem_unlink("forks");
	sem_unlink("output");
	if (args->philos)
		free(args->philos);
	args->philos = NULL;
	args->forks = NULL;
	exit (0);
}
