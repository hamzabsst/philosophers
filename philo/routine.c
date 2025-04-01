/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:07:13 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/01 11:47:02 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	print_msg(philo, FORK);
	usleep(philo->data->t_die * 1000);
	print_msg(philo, DEAD);
	return (NULL);
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
	pthread_mutex_lock(philo->left);
	print_msg(philo, FORK);
	pthread_mutex_lock(philo->right);
	print_msg(philo, FORK);
	print_msg(philo, EAT);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = live_time(philo->data->t_start);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	smart_sleep(philo->data->t_eat, philo->data);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	return (0);
}

static int	philo_sleep(t_philo *philo)
{
	print_msg(philo, SLEEP);
	smart_sleep(philo->data->t_sleep, philo->data);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		print_msg(philo, THINK);
/* 		if (philo->data->philo % 2)
			usleep(1000); */
		if (philo_eat(philo))
			break ;
		if (philo_sleep(philo))
			break ;
	}
	return (NULL);
}
