/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:07:13 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/26 03:10:36 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->write_lock);
	if (!philo->data->end)
		printf("%ld ms: %d %s", live_time(philo->data->t_start), philo->id + 1, msg);
	pthread_mutex_unlock(&philo->data->write_lock);
}


long	live_time(long start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) - start);
}
void	smart_sleep(long duration, t_data *data)
{
	long	start;
	long	elapsed;

	start = live_time(0);
	while (!data->end)
	{
		elapsed = live_time(0) - start;
		if (elapsed >= duration)
			break;
		usleep(100);
	}
}

void	philo_eating(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->forks[philo->id]);
	pthread_mutex_lock(&data->forks[(philo->id + 1) % data->philo]);
	print(philo, FORK);
	print(philo, EAT);
	philo->last_meal = live_time(0);
	philo->meals_eaten++;
	smart_sleep(data->t_eat, data);
	pthread_mutex_unlock(&data->forks[philo->id]);
	pthread_mutex_unlock(&data->forks[(philo->id + 1) % data->philo]);
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
			break;
		}
		if (!data->end)
			printf("%ld ms: %d %s", live_time(data->t_start), philo->id + 1, THINK);
		pthread_mutex_unlock(&data->write_lock);
		philo_eating(philo);
		print(philo, SLEEP);
		smart_sleep(data->t_sleep, data);
		pthread_mutex_lock(&data->write_lock);
		if (data->end)
		{
			pthread_mutex_unlock(&data->write_lock);
			break;
		}
		pthread_mutex_unlock(&data->write_lock);
	}
	return (NULL);
}

void	*monitor_death(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int		i;
	int		done;

	philo = (t_philo *)arg;
	data = philo[0].data;
	while (1)
	{
		done = 1;
		i = -1;
		while (++i < data->philo)
		{
			pthread_mutex_lock(&data->write_lock);
			if ((live_time(0) - philo[i].last_meal) > data->t_die)
			{
				printf("%ld ms: %d %s", live_time(data->t_start), philo[i].id + 1, DEAD);
				data->end = 1;
				pthread_mutex_unlock(&data->write_lock);
				return (NULL);
			}
			if (data->n_eat != -1 && philo[i].meals_eaten < data->n_eat)
				done = 0;
			pthread_mutex_unlock(&data->write_lock);
		}
		if (data->n_eat != -1 && done)
		{
			print(philo, FINISH);
			data->end = 1;
			return (NULL);
		}
		usleep(1000);
	}
}
