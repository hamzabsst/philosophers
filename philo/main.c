/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 02:37:59 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/24 02:10:23 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data		data;
	t_philo		*philo;
	pthread_t	monitor;
	int			i;

	if (parsing(ac, av, &data, &philo))
		return (1);
	if (pthread_create(&monitor, NULL, monitor_death, philo))
		return (printf("Error: monitor thread creation failed\n"), 1);
	i = 0;
	while (i < data.philos)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < data.philos)
	{
		pthread_mutex_destroy(&data.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data.write_lock);
	free(data.forks);
	free(philo);
	return (0);
}
