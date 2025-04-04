/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 02:37:59 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 08:40:55 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
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

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philo;

	if (ac != 5 && ac != 6)
		return (printf("Error: Invalid number of arguments!\n"), 1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (printf("Error: malloc failed\n"), 1);
	if (init_data(data, ac, av))
		return (1);
	philo = malloc(sizeof(t_philo) * data->n_philo);
	if (!philo)
	{
		(free(data->forks), free(data));
		return (printf("Error: malloc failed for philo\n"), 1);
	}
	create_philo(data, philo);
	cleanup(data, philo);
	return (0);
}
