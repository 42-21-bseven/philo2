//
// Created by Brooks Seven on 10/15/21.
//

#include "philo.h"

void 	put_err(char *str)
{
	printf("\n%s\n", str);
	exit(1);
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

void 	init_args(t_args *args, char **av)
{
	args->number_of_philosophers = ft_atoi(av[1]);
	args->time_to_die = ft_atoi(av[2]);
	args->time_to_eat = ft_atoi(av[3]);
	args->time_to_sleep = ft_atoi(av[4]);
	if (av[5] != NULL)
		args->numb_of_meals = ft_atoi(av[5]);
	else
		args->numb_of_meals = 0;

	pthread_mutex_init(&args->output, NULL); //??

}

int 	init_table(t_args *args)
{
	args->philos = malloc(sizeof(t_philos) * args->number_of_philosophers);
	if (!(args->philos))
		put_err("ERROR philos alloc");
	args->forks = malloc(sizeof (pthread_mutex_t) * args->number_of_philosophers);
	if (!args->forks)
		put_err("ERROR forks alloc");
	return (0);
}

int 	init_philo(t_args *args)
{
	int i;

	i = 0;
	while (i < args->number_of_philosophers -1)
	{
		args->philos[i].name_philo = i;
		args->philos[i].left_fork = i;
		args->philos[i].right_fork = i + 1;
		args->philos[i].args = args;
		args->philos[i].last_eat = current_time();
		i++;
	}
	args->philos[i].name_philo = i;
	args->philos[i].left_fork = i;
	args->philos[i].right_fork = 0;
	args->philos[i].args = args;
	args->born_time = current_time();
	args->philos[i].last_eat = current_time();
}

LLU		ft_usleep(int time)
{
	long	t;

	t = current_time();
	while (current_time() - t < time)
		usleep(100);
}

void 	ft_output(t_philos *philos, char *str)
{
	t_args *args;

	args = (t_args *)philos->args;
	pthread_mutex_lock(&args->output);
	printf("      %llu ... philo#%d ... %s\n", (current_time() - args->born_time), philos->name_philo, str);
	pthread_mutex_unlock(&args->output);
}

int 	ft_eat(t_philos *philo)
{
	t_args *args;

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
	if (args->full_saturation < args->numb_of_meals * args->number_of_philosophers)
		args->full_saturation += 1;
	else
		return (1);
	return (0);
}

void	*philos_thread(void *src)
{
	int i;
	int flag;
	t_philos *philo;


	flag = 1;
	philo = (t_philos *)src;

	while (flag == 1)
	{
//		printf("phN: %d while start\n", philo->name_philo);
		if (ft_eat(philo))
			return (0);
		flag = 1;
		ft_usleep(5);
	}
}

void 	*dead_thread(void *src)
{
	int i;
	t_args *dest;

	i = 0;
	dest = (t_args *)src;
	while (1)
	{
		i = 0;
		while (i < dest->number_of_philosophers) {
//		dest->philos[i].last_eat = current_time();
//			printf("\nNAME=>%d ...  L_fork=%u | R_fork=%u ... T_T_D => %d   C_T => %lli   L_E => %lli",
//				   i, dest->philos[i].left_fork, dest->philos[i].right_fork,
//				   dest->time_to_die, current_time(), dest->philos[i].last_eat);
			if (dest->time_to_die <
				(long long) (current_time() - dest->philos[i].last_eat))
			{
				pthread_mutex_lock(&dest->output);
				printf("\n%lli philo#%d is dead\n", current_time() - dest->born_time, i);
				return (NULL);
			}
			i++;
		}
	}
	printf("\n");
}

int 	create_threads(t_args *args)
{
	int i = 0;
	if (pthread_create(&args->dead_thread, NULL, dead_thread, args) != 0)
		put_err("ERROR thread");

	while (i < args->number_of_philosophers)
		pthread_mutex_init(&args->forks[i++], NULL);
	i = 0;
	while (i < args->number_of_philosophers)
	{
//		pthread_mutex_init(&args->forks[i], NULL);
		pthread_create(&args->philos[i].ph_thread, NULL, philos_thread, &args->philos[i]);
		i++;
		ft_usleep(1);
//		usleep(22);
	}
/*	i = 1;
	while (i < args->number_of_philosophers)
	{
//		pthread_mutex_init(&args->forks[i], NULL);
		pthread_create(&args->philos[i].ph_thread, NULL, philos_thread, &args->philos[i]);
		i += 2;
//		usleep(17);
	}*/

}

int 	main(int ac, char **av)
{
	t_args args;

	int i = 0;

	if (check_args(ac, av) < 0)
		put_err("ERROR in args!");
	printf("\nHello World! = %lli\n\n", current_time());
	while (i < ac)
		printf("arg%d = %s\n", i, av[i++]);
	init_args(&args, av);
	printf("\nint_args == %d,%d,%d,%d,%d\n\n", args.number_of_philosophers, args.time_to_die, args.time_to_eat, args.time_to_sleep, args.numb_of_meals);
	init_table(&args);
	i = 0;
//	while (i < args.number_of_philosophers)
//		printf("philo#%d    l.fork -> %d | %d <- r.fork\n", args.philos[i].name_philo, args.philos[i].left_fork, args.philos[i++].right_fork);

	init_philo(&args);
	create_threads(&args);

//	i = 0;
//	while (i < args.number_of_philosophers)
//	{
//		pthread_mutex_init(&args.forks[i], NULL);
//		if (pthread_create(&args.philos[i].ph_thread, NULL, &philos_thread, &args.philos[i]) != 0)
//			return (1);
//		i++;
//		usleep(130);
//
//	}
//	i = 1;
//	while (i < args.number_of_philosophers)
//	{
//		pthread_mutex_init(&args.forks[i], NULL);
//		if (pthread_create(&args.philos[i].ph_thread, NULL, &philos_thread, &args.philos[i]) != 0)
//			return (1);
//		i += 2;
//		usleep(130);
//
//	}

//	pause();
	pthread_join(args.dead_thread, NULL);

//	pthread_create(&args.main_tread, NULL, main_tread, (void *)&args);


//	init_philo(&args);
//	i = 0;
//	while (i < args.number_of_philosophers)
//		printf("philo#%d    l.fork -> %d | %d <- r.fork\n", args.philos[i].name_philo, args.philos[i].left_fork, args.philos[i++].right_fork);
	return 0;
}
