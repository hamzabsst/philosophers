/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 06:34:59 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/30 06:37:07 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	live_time(long start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) - start);
}

void	smart_sleep(long duration, t_data *data)
{
	long	start;

	start = live_time(data->t_start);
	while (live_time(data->t_start) - start < duration)
	{
		usleep(50);
		pthread_mutex_lock(&data->end_mutex);
		if (data->end)
		{
			pthread_mutex_unlock(&data->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->end_mutex);
	}
}

void	print_msg(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->end_mutex);
	if (!philo->data->end)
	{
		pthread_mutex_lock(&philo->data->write_lock);
		printf("%ld ms: [%d] %s",
			live_time(philo->data->t_start), philo->id + 1, msg);
		pthread_mutex_unlock(&philo->data->write_lock);
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
}

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
