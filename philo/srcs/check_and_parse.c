#include "../includes/philo.h"

int	ft_atoi(char *str)
{
	long	res;

	res = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		if (res < 2147483647)
			res = res * 10 + (*str++ - '0');
		else
			return (-2);
	}
	if (res > 2147483647)
		return (-2);
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
	if (args.numb_of_meals < -1)
		return (put_err("! Wrong numder of meals."));
	if (args.numb_of_meals == 0)
		return (put_err(". These philosophers are fed up."));
	return (0);
}
