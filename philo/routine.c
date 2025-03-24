/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:07:13 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/24 02:12:02 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void smart_sleep(long duration)
{
	long start = get_time();
	while ((get_time() - start) < duration)
		usleep(100);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&data->write_lock);
		if (data->end)
		{
			pthread_mutex_unlock(&data->write_lock);
			break ;
		}
		pthread_mutex_unlock(&data->write_lock);
		printf("%ld ms: %d %s\n", get_time() - data->start_time, philo->id + 1, THNK);
		pthread_mutex_lock(&data->forks[philo->id]);
		pthread_mutex_lock(&data->forks[(philo->id + 1) % data->philos]);
		printf("%ld ms: %d %s\n", get_time() - data->start_time, philo->id + 1, EAT);
		philo->last_meal = get_time();
		philo->meals_eaten++;
		smart_sleep(data->t_eat);
		pthread_mutex_unlock(&data->forks[philo->id]);
		pthread_mutex_unlock(&data->forks[(philo->id + 1) % data->philos]);
		printf("%ld ms: %d %s\n",get_time() - data->start_time, philo->id + 1, SLP);
		smart_sleep(data->t_sleep);
	}
	return (NULL);
}

void	*monitor_death(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		i;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (1)
	{
		i = 0;
		while (i < data->philos)
		{
			pthread_mutex_lock(&data->write_lock);
			if (data->end)
			{
				pthread_mutex_unlock(&data->write_lock);
				return (NULL);
			}
			if ((get_time() - philos[i].last_meal) > data->t_die)
			{
				printf("%ld ms: %d %s\n", get_time() - data->start_time, philos[i].id + 1, DIED);
				data->end = 1;
				pthread_mutex_unlock(&data->write_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->write_lock);
			i++;
		}
		usleep(1000);
	}
}
