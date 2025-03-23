/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:14:37 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/23 19:45:58 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atol(const char *str, int *result)
{
	long	nbr;
	int		i;

	nbr = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] < '0' || str[i] > '9')
		return (1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (nbr > (INT_MAX - (str[i] - '0')) / 10)
			return (1);
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (1);
	*result = (int)nbr;
	return (0);
}

static int	init_variables(int ac, char **av, t_data *data)
{
	int	temp;

	if (ft_atol(av[1], &temp) || temp <= 0)
		return (printf("Error: Invalid number of philosophers\n"), 1);
	data->philos = temp;
	if (ft_atol(av[2], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_die\n"), 1);
	data->t_die = temp;
	if (ft_atol(av[3], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_eat\n"), 1);
	data->t_eat = temp;
	if (ft_atol(av[4], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_sleep\n"), 1);
	data->t_sleep = temp;
	if (ac == 6)
	{
		if (ft_atol(av[5], &temp) || temp <= 0)
			return (printf("Error: Invalid times_each_philos_must_eat\n"), 1);
		data->n_eat = temp;
	}
	else
		data->n_eat = -1;
	data->start_time = get_time();
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos);
	if (!data->forks)
		return (printf("Error: malloc failed\n"), 1);
	i = 0;
	while (i < data->philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (printf("Error: mutex init failed\n"), 1);
		i++;
	}
	if (pthread_mutex_init(&data->write_lock, NULL))
		return (printf("Error: mutex init failed\n"), 1);
	return (0);
}

int	init_philos(t_data *data, t_philo **philos)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * data->philos);
	if (!*philos)
		return (printf("Error: malloc failed\n"), 1);
	i = 0;
	while (i < data->philos)
	{
		(*philos)[i].id = i;
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].last_meal = 0;
		(*philos)[i].data = data;
		if (pthread_create(&(*philos)[i].thread, NULL, routine, &(*philos)[i]))
			return (printf("Error: thread creation failed\n"), 1);
		i++;
	}
	return (0);
}

int	parsing(int ac, char **av, t_data *data, t_philo **philo)
{
	if (ac != 5 && ac != 6)
		return (printf("Error: Invalid number of arguments!\n"), 1);
	if (init_variables(ac, av, data) || init_philos(data, philo) || init_forks(data))
		return (1);
	return (0);
}
