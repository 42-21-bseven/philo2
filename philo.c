//
// Created by Brooks Seven on 10/15/21.
//

#include "philo.h"

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

int	ft_atoi(char *str)
{
	long	res;

	res = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		if (res < 2147483648)
			res = res * 10 + (*str++ - '0');
		else
			return (-1);
	}
	if (res > 2147483647)
		return (-1);
	return ((int)res);
}

int	chk_not_num(char *str)
{
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			str++;
		else
			return (-1);
	}
	return (0);
}

int	check_args(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac < 5 || ac > 6)
		return (-1);
	while (i < ac)
	{
		if (chk_not_num(av[i++]) < 0)
			return (-1);
	}
	return (0);
}

void	parse_args(t_args *args, char **av)
{
	args->number_of_philosophers = ft_atoi(av[1]);
	args->time_to_die = ft_atoi(av[2]);
	args->time_to_eat = ft_atoi(av[3]);
	args->time_to_sleep = ft_atoi(av[4]);
	if (av[5] != NULL)
		args->numb_of_meals = ft_atoi(av[5]);
	else
		args->numb_of_meals = -1;
}

void	init_null(t_args *args)
{
	args->forks = NULL;
	args->philos = NULL;
	args->output = NULL;
	args->pid = NULL;
}

int	check_vol(t_args args)
{
	if (args.number_of_philosophers < 1 || args.number_of_philosophers > 200)
		return (put_err("! Wrong number of philosophers."));
	if (args.time_to_die < 1)
		return (put_err("! Wrong time to die."));
	if (args.time_to_eat < 1)
		return (put_err("! Wrong meal time."));
	if (args.time_to_sleep < 1)
		return (put_err("! Wrong sleep to time."));
	if (args.numb_of_meals == 0)
		return (put_err(". These philosophers are fed up."));
	return (0);
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

void	ft_usleep(int time)
{
	LLU	t;

	t = current_time();
	while (current_time() - t < time)
		usleep(100);
}

void	ft_output(t_philos *philos, char *str)
{
	sem_wait(philos->args->output);
	printf("%llu\tphilo#%d \t...\t %s\n",
		(current_time() - philos->args->born_time), philos->name_philo, str);
	sem_post(philos->args->output);
}

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
	{
		exit(0);
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
