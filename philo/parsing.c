/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:14:37 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/03 11:24:05 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(const char *str, int *result)
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

	if (ft_atoi(av[1], &temp) || temp <= 0)
		return (printf("Error: Invalid number of philosophers\n"), 1);
	data->philo = temp;
	if (ft_atoi(av[2], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_die\n"), 1);
	data->t_die = temp;
	if (ft_atoi(av[3], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_eat\n"), 1);
	data->t_eat = temp;
	if (ft_atoi(av[4], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_sleep\n"), 1);
	data->t_sleep = temp;
	if (ac == 6)
	{
		if (ft_atoi(av[5], &temp) || temp <= 0)
			return (printf("Error: Invalid times_each_philos_must_eat\n"), 1);
		data->n_eat = temp;
	}
	else
		data->n_eat = -1;
	data->t_start = live_time(0);
	data->end = 0;
	return (0);
}

int	init_data(t_data *data, int ac, char **av)
{
	int	i;

	if (init_variables(ac, av, data))
		return (free(data), 1);
	pthread_mutex_init(&data->end_mutex, NULL);
	pthread_mutex_init(&data->write_lock, NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo);
	if (!data->forks)
	{
		free(data);
		return (printf("Error: malloc failed for forks\n"), 1);
	}
	i = 0;
	while (i < data->philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (0);
}

static void	init_philo(t_data *data, t_philo *philo, int i)
{
	pthread_mutex_init(&philo[i].meal_mutex, NULL);
	philo[i].id = i + 1;
	philo[i].meals_eaten = 0;
	philo[i].data = data;
	philo[i].last_meal = data->t_start;
	philo[i].right = &data->forks[i];
	philo[i].left = &data->forks[(i + 1) % data->philo];
}

void	create_philo(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->philo)
	{
		init_philo(data, philo, i);
		pthread_create(&philo[i].thread, NULL, routine, &philo[i]);
		i++;
	}
	if (data->philo > 1)
		monitoring(philo);
	while (--i >= 0)
		pthread_join(philo[i].thread, NULL);
}
