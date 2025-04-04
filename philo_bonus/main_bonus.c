/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 07:56:29 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 07:56:11 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sem_unlinking(void)
{
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/death");
	sem_unlink("/limit");
	sem_unlink("/meal_mutex");
}

void	cleanup(t_data *data, t_philo *philo)
{
	sem_close(data->forks);
	sem_close(data->write);
	sem_close(data->death);
	sem_close(data->eat_limit);
	sem_close(data->meal_mutex);
	sem_unlinking();
	if (philo)
	{
		if (philo->pids)
			free(philo->pids);
		free(philo);
	}
	if (data)
		free(data);
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
		return (free(data), 1);
	philo = malloc(sizeof(t_philo) * data->philo);
	if (!philo)
	{
		cleanup(data, philo);
		return (printf("Error: malloc failed for philo\n"), 1);
	}
	philo->pids = malloc(sizeof(pid_t) * data->philo);
	if (!philo->pids)
	{
		cleanup(data, philo);
		return (printf("Error: malloc failed for pids\n"), 1);
	}
	create_philos(data, philo);
	cleanup(data, philo);
	return (0);
}
