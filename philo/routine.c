/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:07:13 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 08:40:55 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->end_mutex);
	if (!philo->data->end)
	{
		pthread_mutex_lock(&philo->data->write_lock);
		printf("%ld\t%d\t%s", live_time(philo->data->t_start), philo->id, msg);
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

static int	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->end_mutex);
	if (philo->data->end)
	{
		pthread_mutex_unlock(&philo->data->end_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
	pthread_mutex_lock(philo->right);
	print_msg(philo, FORK);
	pthread_mutex_lock(philo->left);
	print_msg(philo, FORK);
	print_msg(philo, EAT);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = live_time(philo->data->t_start);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	usleep(philo->data->t_eat * 1000);
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->n_philo == 1)
	{
		printf("%ld\t%d\t%s", live_time(philo->data->t_start), 1, FORK);
		usleep(philo->data->t_die * 1000);
		printf("%ld\t%d\t%s", live_time(philo->data->t_start), 1, DEAD);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		print_msg(philo, THINK);
		if (philo->data->n_philo % 2)
			usleep(1000);
		if (philo_eat(philo))
			break ;
		print_msg(philo, SLEEP);
		usleep(philo->data->t_sleep * 1000);
	}
	return (NULL);
}
