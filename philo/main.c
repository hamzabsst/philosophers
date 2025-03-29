/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 02:37:59 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/29 19:32:15 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->end_mutex);
	if (data->forks)
		free(data->forks);
	free(data);
	free(philo);
}

int	create_philo(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	if (data->philo == 1)
	{
		if (pthread_mutex_init(&philo[i].meal_mutex, NULL) != 0)
		{
			cleanup(data, philo);
			return (printf("Error: meal_mutex init failed\n"), 1);
		}
		philo[i].id = i;
		philo[i].meals_eaten = 0;
		philo[i].data = data;
		philo[i].last_meal = live_time(data->t_start);
		if (pthread_create(&philo[i].thread, NULL, one_philo, &philo[i]))
		{
			cleanup(data, philo);
			return (printf("Error: thread creation failed\n"), 1);
		}
		pthread_join(philo[i].thread, NULL);
	}
	else if (data->philo > 1)
	{
		while (i < data->philo)
		{
			if (pthread_mutex_init(&philo[i].meal_mutex, NULL) != 0)
			{
				cleanup(data, philo);
				return (printf("Error: meal_mutex init failed\n"), 1);
			}
			philo[i].id = i;
			philo[i].meals_eaten = 0;
			philo[i].data = data;
			philo[i].last_meal = data->t_start;
			if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]))
			{
				cleanup(data, philo);
				return (printf("Error: thread creation failed\n"), 1);
			}
			i++;
		}
		if (create_thread(philo))
		{
			cleanup(data, philo);
			return (1);
		}
		i = 0;
		while (i < data->philo)
		{
			pthread_join(philo[i].thread, NULL);
			i++;
		}
	}
	cleanup(data, philo);
	return(0);
}

int main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philo;
	int		i;

	data = malloc(sizeof(t_data));
	if (!data)
		return (printf("Error: malloc failed\n"), 1);
	if (parsing(ac, av, data))
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
	philo = malloc(sizeof(t_philo) * data->philo);
	if (!philo)
	{
		free(data->forks);
		free(data);
		return (printf("Error: malloc failed for philo\n"), 1);
	}
	create_philo(data, philo);
	return (0);
}
