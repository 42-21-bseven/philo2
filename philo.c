//
// Created by Brooks Seven on 10/15/21.
//

#include "philo.h"

int 	put_err(char *str)
{
	printf("\nERROR%s\n", str);
	return (1);

}

LLU		current_time()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int		ft_atoi(char *str)
{
	int res;

	res = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		res = res * 10 + (*str++ - '0');
	}
	if (res > 2147483647 || res < 0)
		return (-1);
	return (res);
}

int 	chk_not_num(char *str)
{
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			str++;
		else
			return (-1);
	}
	return 0;
}

int 	check_args(int ac, char **av)
{
	int i;

	i = 1;
	if (ac < 5 || ac > 6)
		return (-1);
	while (i < ac)
	{
		if (chk_not_num(av[i++]) < 0)
			return (-1);
	}
	return 0;
}

void 	parse_args(t_args *args, char **av)
{
	args->number_of_philosophers = ft_atoi(av[1]);
	args->time_to_die = ft_atoi(av[2]);
	args->time_to_eat = ft_atoi(av[3]);
	args->time_to_sleep = ft_atoi(av[4]);
	if (av[5] != NULL)
		args->numb_of_meals = ft_atoi(av[5]);
	else
		args->numb_of_meals = -1;
//	pthread_mutex_init(&args->output, NULL); //??
}

int 	check_vol(t_args args)
{
	if (args.number_of_philosophers < 2 || args.number_of_philosophers > 200)
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

int 	init_table(t_args *args)
{
	args->philos = malloc(sizeof(t_philos) * args->number_of_philosophers);
	if (!(args->philos))
		return (put_err(" philos alloc."));
	args->forks = malloc(sizeof (pthread_mutex_t) * args->number_of_philosophers);
	if (!args->forks)
		return (put_err(" forks alloc."));
	return (0);
}

void 	init_philo(t_args *args)
{
	int i;

	i = 0;
	while (i < args->number_of_philosophers -1)
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

void		ft_usleep(int time)
{
	LLU	t;

	t = current_time();
	while (current_time() - t < time)
		usleep(100);
}

void 	ft_output(t_philos *philos, char *str)
{
	t_args *args;

	args = (t_args *)philos->args;
	sem_wait(args->output);
	printf("%llu\tphilo#%d \t...\t %s\n", (current_time() - args->born_time), philos->name_philo, str);
	sem_post(args->output);
}

int 	ft_eat(t_philos *philo)
{
	t_args *args;

	args = (t_args *)philo->args;
	sem_wait(args->forks);
	ft_output(philo, "take LEFT fork");
	sem_wait(args->forks);
	ft_output(philo, "take RIGHT fork");
	ft_output(philo, "is OM NOM NOM doing");
	philo->last_eat = current_time();
	ft_usleep(args->time_to_eat);
	sem_post(args->forks);
	ft_output(philo, "PUT the left fork");
	sem_post(args->forks);
	ft_output(philo, "PUT the right fork");
	if (philo->full_saturation < args->numb_of_meals)
		philo->full_saturation++;
	if (philo->full_saturation == args->numb_of_meals)
		exit (0);
	return (0);
}

void	*philos_thread(void *src)
{
	t_philos *philo;

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

int 	*dead_thread(t_args *dest)
{
	int i;
	int k;

	while (1)
	{
		i = 0;
		k = 0;
		while (i < dest->number_of_philosophers) {
			if (dest->time_to_die <	(long long) (current_time() - dest->philos->last_eat))
			{
				sem_wait(dest->output);
				printf("%lli philo#%d is dead\n", current_time() - dest->born_time, dest->philos->name_philo);
				ft_usleep(10);
				exit (0);
			}
			k = k + dest->philos->full_saturation;
			if (k == dest->number_of_philosophers * dest->numb_of_meals)
				exit (0);
			i++;
		}
	}
}

int 	create_threads(t_args *args)
{
	int i;

	sem_unlink("forks");
	sem_unlink("output");
	args->output = sem_open("output", O_CREAT, 0666, 1);
	args->forks = sem_open("forks", O_CREAT, 0666, args->number_of_philosophers);
	i = 0;
	args->pid = malloc(sizeof (pid_t) * args->number_of_philosophers);
	while (i < args->number_of_philosophers)
	{
		args->philos[i].pid = args->pid;
		args->pid[i] = fork();
		if (args->pid[i] < 0)
			return (1);
		else if (args->pid[i] == 0)
		{
			args->born_time = current_time();
			if (pthread_create(&args->philos[i].ph_thread, NULL, philos_thread, &args->philos[i]) != 0)
				return (put_err(" thread create."));
			dead_thread(args->philos);
		}
		i++;

//		ft_usleep(4);
	}
	waitpid(-1, NULL, 0);
	return (0);
}

void		all_free(t_args *args)
{
	int i;

	i = 0;
	while(i < args->number_of_philosophers)
		kill(args->pid[i++], SIGINT);
	sem_unlink("forks");
	sem_unlink("output");
	if (args->philos)
		free(args->philos);
	args->philos = NULL;
	args->forks = NULL;
	exit (0);
}

int 	main(int ac, char **av)
{
	t_args args;

	if (check_args(ac, av) < 0)
		return (put_err(" in args!"));
	parse_args(&args, av);
	if (check_vol(args))
		return (1);
	if (init_table(&args))
		return (1);
	init_philo(&args);
	if (create_threads(&args))
		return (1);
	all_free(&args);
	return 0;
}
