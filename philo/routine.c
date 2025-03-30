/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:07:13 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/30 08:17:13 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks_even(t_philo *philo, int left_fork, int right_fork)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->forks[left_fork]);
	print_msg(philo, L_FORK);
	if (live_time(data->t_start) - philo->last_meal >= data->t_die)
	{
		pthread_mutex_unlock(&data->forks[left_fork]);
		return (0);
	}
	pthread_mutex_lock(&data->forks[right_fork]);
	print_msg(philo, R_FORK);
	return (1);
}

static int	take_forks_odd(t_philo *philo, int left_fork, int right_fork)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->forks[right_fork]);
	print_msg(philo, R_FORK);
	if (live_time(data->t_start) - philo->last_meal >= data->t_die)
	{
		pthread_mutex_unlock(&data->forks[right_fork]);
		return (0);
	}
	pthread_mutex_lock(&data->forks[left_fork]);
	print_msg(philo, L_FORK);
	return (1);
}

void	philo_eating(t_philo *philo)
{
	t_data	*data;
	int		right_fork;
	int		left_fork;

	data = philo->data;
	right_fork = philo->id;
	left_fork = (philo->id + 1) % data->philo;
	if (philo->id % 2 == 0)
	{
		if (!take_forks_even(philo, left_fork, right_fork))
			return ;
	}
	else
	{
		if (!take_forks_odd(philo, left_fork, right_fork))
			return ;
	}
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = live_time(philo->data->t_start);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	(print_msg(philo, EAT), smart_sleep(data->t_eat, data));
	pthread_mutex_unlock(&data->forks[left_fork]);
	pthread_mutex_unlock(&data->forks[right_fork]);
	usleep(500);
}

void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	print_msg(philo, R_FORK);
	usleep(philo->data->t_die * 1000);
	print_msg(philo, DEAD);
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2)
		usleep(data->t_eat * 1000);
	while (1)
	{
		pthread_mutex_lock(&data->end_mutex);
		if (data->end)
		{
			pthread_mutex_unlock(&data->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->end_mutex);
		philo_eating(philo);
		print_msg(philo, SLEEP);
		smart_sleep(data->t_sleep, data);
		print_msg(philo, THINK);
	}
	return (NULL);
}
