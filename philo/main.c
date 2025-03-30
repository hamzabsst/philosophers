/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 02:37:59 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/30 07:12:54 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	philo = malloc(sizeof(t_philo) * data->philo);
	if (!philo)
	{
		free(data->forks);
		free(data);
		return (printf("Error: malloc failed for philo\n"), 1);
	}
	if (create_philo(data, philo))
		return (1);
	return (0);
}
