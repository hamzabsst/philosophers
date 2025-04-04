/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 07:56:56 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/03 20:50:24 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	monitoring(t_philo *philo)
{
	long	current_time;
	long	last_meal_copy;
	int		meals_eaten_copy;

	while (1)
	{
		sem_wait(philo->data->meal_mutex);
		last_meal_copy = philo->last_meal;
		meals_eaten_copy = philo->meals_eaten;
		sem_post(philo->data->meal_mutex);
		current_time = live_time(philo->data->t_start);
		if (current_time - last_meal_copy >= philo->data->t_die)
		{
			print_msg(philo, DEAD, 0);
			sem_post(philo->data->death);
			return;
		}
		if (meals_eaten_copy >= philo->data->n_eat && philo->data->n_eat > 0)
		{
			sem_post(philo->data->eat_limit);
			return;
		}
		usleep(1000);
	}
}

void *second_monitor(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	int i = 0;

	while (i < philo->data->philo)
	{
		sem_wait(philo->data->eat_limit);
		i++;
		if (i == philo->data->philo)
			break;
		usleep(1000);
	}
	sem_post(philo->data->death);
	return NULL;
}


void	monitore(t_philo *philo)
{
	int	i;
	int	status;

	sem_wait(philo->data->death);
	i = 0;
	while (i < philo->data->philo)
	{
		if (philo->pids[i] > 0)
			kill(philo->pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < philo->data->philo)
	{
		waitpid(philo->pids[i], &status, 0);
		i++;
	}
}
