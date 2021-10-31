/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bseven <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 12:59:38 by bseven            #+#    #+#             */
/*   Updated: 2021/10/31 12:59:40 by bseven           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Brooks Seven on 10/15/21.
//

#include "../includes/philo.h"

int	ft_eat(t_philos *philo)
{
	t_args	*args;

	args = (t_args *)philo->args;
	pthread_mutex_lock(&args->forks[philo->left_fork]);
	ft_output(philo, "take LEFT fork");
	pthread_mutex_lock(&args->forks[philo->right_fork]);
	ft_output(philo, "take RIGHT fork");
	ft_output(philo, "is OM NOM NOM doing");
	philo->last_eat = current_time();
	ft_usleep(args->time_to_eat);
	pthread_mutex_unlock(&args->forks[philo->left_fork]);
	ft_output(philo, "PUT the left fork");
	pthread_mutex_unlock(&args->forks[philo->right_fork]);
	ft_output(philo, "PUT the right fork");
	if (philo->full_saturation < args->numb_of_meals)
		philo->full_saturation++;
	if (philo->full_saturation == args->numb_of_meals)
		return (1);
	return (0);
}

void	*philos_thread(void *src)
{
	t_philos	*philo;

	philo = (t_philos *)src;
	while (1)
	{
		if (ft_eat(philo))
			return (0);
		ft_output(philo, "is sleeping.");
		ft_usleep(philo->args->time_to_sleep);
		ft_output(philo, "is thinking.");
		ft_usleep(5);
	}
}

void	*dead_thread(void *src)
{
	int		i;
	int		k;
	t_args	*dest;

	dest = (t_args *)src;
	while (1)
	{
		i = 0;
		k = 0;
		while (i < dest->number_of_philosophers)
		{
			if (dest->time_to_die
				< (long long)(current_time() - dest->philos[i].last_eat))
			{
				pthread_mutex_lock(&dest->output);
				printf("\n%lli philo#%d is dead\n",
					   current_time() - dest->born_time, i + 1);
				return (NULL);
			}
			k = k + dest->philos[i].full_saturation;
			if (k == dest->number_of_philosophers * dest->numb_of_meals)
				return (NULL);
			i++;
		}
	}
}

int	create_threads(t_args *args)
{
	int	i;

	i = 0;
	if (pthread_create(&args->dead_thread, NULL, dead_thread, args) != 0)
		return (put_err(" thread create."));
	while (i < args->number_of_philosophers)
		pthread_mutex_init(&args->forks[i++], NULL);
	i = 0;
	while (i < args->number_of_philosophers)
	{
		if (pthread_create(&args->philos[i].ph_thread, NULL, philos_thread,
				&args->philos[i]) != 0)
			return (put_err(" thread create."));
		i++;
		ft_usleep(1);
	}
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
	else
		pthread_join(args.dead_thread, NULL);
	all_free(&args);
	return (0);
}
