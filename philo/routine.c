/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:07:13 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/26 18:21:56 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->end_mutex);
	if (!philo->data->end)
	{
		pthread_mutex_lock(&philo->data->write_lock);
		printf("%ld ms: %d %s",
			live_time(philo->data->t_start), philo->id + 1, msg);
		pthread_mutex_unlock(&philo->data->write_lock);
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
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

	start = live_time(0);
	while (live_time(0) - start < duration)
	{
		usleep(100);
		pthread_mutex_lock(&data->end_mutex);
		if (data->end)
		{
			pthread_mutex_unlock(&data->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->end_mutex);
	}
}

void	philo_eating(t_philo *philo)
{
	t_data	*data;
	int		right_fork;
	int		left_fork;
	int		temp;

	data = philo->data;
	right_fork = philo->id;
	left_fork = (philo->id + 1) % data->philo;
	if (right_fork > left_fork)
	{
		temp = right_fork;
		right_fork = left_fork;
		left_fork = temp;
	}
	pthread_mutex_lock(&data->forks[right_fork]);
	print_msg(philo, R_FORK);
	pthread_mutex_lock(&data->forks[left_fork]);
	(print_msg(philo, L_FORK), print_msg(philo, EAT));
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = live_time(0);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	smart_sleep(data->t_eat, data);
	pthread_mutex_unlock(&data->forks[left_fork]);
	pthread_mutex_unlock(&data->forks[right_fork]);
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
		pthread_mutex_lock(&data->end_mutex);
		if (data->end)
		{
			pthread_mutex_unlock(&data->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->end_mutex);
		print_msg(philo, THINK);
		philo_eating(philo);
		print_msg(philo, SLEEP);
		smart_sleep(data->t_sleep, data);
	}
	return (NULL);
}

void	*monitor_death(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		i;
	int		all_done;
	long	time_since_meal;
	int		meals;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (1)
	{
		i = 0;
		all_done = 1;
		while (i < data->philo)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			time_since_meal = live_time(0) - philos[i].last_meal;
			meals = philos[i].meals_eaten;
			pthread_mutex_unlock(&philos[i].meal_mutex);
			if (time_since_meal > data->t_die)
			{
				pthread_mutex_lock(&data->end_mutex);
				if (!data->end)
				{
					pthread_mutex_lock(&data->write_lock);
					printf("%ld ms: %d %s",
						live_time(data->t_start), i + 1, DEAD);
					pthread_mutex_unlock(&data->write_lock);
					data->end = 1;
				}
				pthread_mutex_unlock(&data->end_mutex);
				return (NULL);
			}
			if (data->n_eat != -1 && meals < data->n_eat)
				all_done = 0;
			i++;
		}
		if (data->n_eat != -1 && all_done)
		{
			pthread_mutex_lock(&data->end_mutex);
			if (!data->end)
			{
				pthread_mutex_lock(&data->write_lock);
				printf("%ld ms: %s", live_time(data->t_start), FINISH);
				pthread_mutex_unlock(&data->write_lock);
				data->end = 1;
			}
			pthread_mutex_unlock(&data->end_mutex);
			return (NULL);
		}
		usleep(1000);
	}
}
