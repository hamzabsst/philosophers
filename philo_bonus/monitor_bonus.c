/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 07:56:56 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/03 07:59:53 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	monitoring(t_philo *philo)
{
	long	current_time;

	while (1)
	{
		current_time = live_time(philo->data->t_start);
		if (current_time - philo->last_meal >= philo->data->t_die)
		{
			print_msg(philo, DEAD);
			sem_post(philo->data->death);
			return ;
		}
		if (philo->meals_eaten >= philo->data->n_eat
			&& philo->data->n_eat > 0)
		{
			sem_post(philo->data->eat_limit);
			return ;
		}
		usleep(1000);
	}
}

void	*second_monitor(void *arg)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = (t_philo *)arg;
	while (i < philo->data->philo)
	{
		sem_wait(philo->data->eat_limit);
		i++;
	}
	sem_post(philo->data->death);
	return (NULL);
}

void	monitore(t_philo *philo)
{
	int	i;

	i = 0;
	sem_wait(philo->data->death);
	while (i++ <= philo->data->philo + 1)
		sem_post(philo->data->eat_limit);
	i = 0;
	while (i < philo->data->philo)
	{
		kill(philo->pids[i], SIGINT);
		i++;
	}
	return ;
}
