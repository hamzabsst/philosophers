/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 07:57:54 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/03 07:59:36 by hbousset         ###   ########.fr       */
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

static void	smart_sleep(long duration, t_data *data)
{
	long	start;

	start = live_time(data->t_start);
	while (live_time(data->t_start) - start < duration)
		usleep(500);
}

static void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_wait(data->forks);
	print_msg(philo, FORK);
	sem_wait(data->forks);
	print_msg(philo, FORK);
	print_msg(philo, EAT);
	philo->last_meal = live_time(data->t_start);
	philo->meals_eaten++;
	smart_sleep(data->t_eat, data);
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
		if (data->philo % 2 == 0)
			usleep(1000);
		while (1)
		{
			print_msg(philo, THINK);
			eat(philo);
			print_msg(philo, SLEEP);
			smart_sleep(data->t_sleep, data);
		}
	}
	return (NULL);
}
