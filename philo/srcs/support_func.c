/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bseven <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 12:59:42 by bseven            #+#    #+#             */
/*   Updated: 2021/10/31 12:59:44 by bseven           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
	t_args	*args;

	args = (t_args *)philos->args;
	pthread_mutex_lock(&args->output);
	printf("%llu\tphilo#%d \t...\t %s\n",
		   (current_time() - args->born_time), philos->name_philo, str);
	pthread_mutex_unlock(&args->output);
}

void	all_free(t_args *args)
{
	if (args->forks)
		pthread_mutex_destroy(args->forks);
	if (args->philos)
		free(args->philos);
	if (args->forks)
		free(args->forks);
	args->philos = NULL;
	args->forks = NULL;
}
