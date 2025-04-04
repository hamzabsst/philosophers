/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 07:57:54 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 07:57:59 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_msg(t_philo *philo, char *msg, int release_sem)
{
	sem_wait(philo->data->write);
	printf("%ld\t%d\t%s", live_time(philo->data->t_start), philo->id, msg);
	if (release_sem)
		sem_post(philo->data->write);
}

long	live_time(long start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) - start);
}

static void eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_wait(data->forks);
	print_msg(philo, FORK, 1);
	sem_wait(data->forks);
	print_msg(philo, FORK, 1);
	print_msg(philo, EAT, 1);
	sem_wait(data->meal_mutex);
	philo->last_meal = live_time(data->t_start);
	philo->meals_eaten++;
	sem_post(data->meal_mutex);
	usleep(data->t_eat * 1000);
	sem_post(data->forks);
	sem_post(data->forks);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->data->philo == 1)
	{
		printf("%ld\t%d\t%s", live_time(philo->data->t_start), 1, FORK);
		usleep(philo->data->t_die * 1000);
		printf("%ld\t%d\t%s", live_time(philo->data->t_start), 1, DEAD);
		return (NULL);
	}
	else
	{
		while (1)
		{
			print_msg(philo, THINK, 1);
			if (philo->data->philo % 2)
				usleep(1000);
			eat(philo);
			print_msg(philo, SLEEP, 1);
			usleep(data->t_sleep * 1000);
		}
	}
	return (NULL);
}
