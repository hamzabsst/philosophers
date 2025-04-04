/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 07:57:54 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 11:18:52 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_msg(t_philo *philo, char *msg)
{
	sem_wait(philo->data->write);
	printf("%ld\t%d\t%s", live_time(philo->data->t_start), philo->id, msg);
	sem_post(philo->data->write);
}

long	live_time(long start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) - start);
}

static void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_wait(data->queue);
	sem_wait(data->forks);
	print_msg(philo, FORK);
	sem_wait(data->forks);
	print_msg(philo, FORK);
	print_msg(philo, EAT);
	philo->last_meal = live_time(data->t_start);
	philo->meals_eaten++;
	usleep(data->t_eat * 1000);
	sem_post(data->forks);
	sem_post(data->forks);
	sem_post(data->queue);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->n_philo == 1)
	{
		print_msg(philo, FORK);
		usleep(philo->data->t_die * 1000);
		print_msg(philo, DEAD);
		return (NULL);
	}
	else
	{
		if (philo->id % 2 == 0)
			usleep(1000);
		while (1)
		{
			print_msg(philo, THINK);
			if (philo->data->n_philo % 2)
				usleep(1000);
			eat(philo);
			print_msg(philo, SLEEP);
			usleep(philo->data->t_sleep * 1000);
		}
	}
	return (NULL);
}
