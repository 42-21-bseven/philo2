#include "../includes/philo.h"

void	init_null(t_args *args)
{
	args->forks = NULL;
	args->philos = NULL;
	pthread_mutex_init(&args->output, NULL);
}

int	init_table(t_args *args)
{
	args->philos = malloc(sizeof(t_philos) * args->number_of_philosophers);
	if (!(args->philos))
		return (put_err(" philos alloc."));
	args->forks = malloc(
			sizeof(pthread_mutex_t) * args->number_of_philosophers);
	if (!args->forks)
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
		args->philos[i].left_fork = i;
		args->philos[i].right_fork = i + 1;
		args->philos[i].args = args;
		args->philos[i].last_eat = current_time();
		args->philos[i].full_saturation = 0;
		i++;
	}
	args->philos[i].name_philo = i + 1;
	args->philos[i].left_fork = i;
	args->philos[i].right_fork = 0;
	args->philos[i].args = args;
	args->born_time = current_time();
	args->philos[i].last_eat = current_time();
	args->philos[i].full_saturation = 0;
}
