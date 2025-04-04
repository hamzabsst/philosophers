/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 07:56:56 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 11:16:29 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	monitor(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	while (1)
	{
		if (live_time(data->t_start) - philo->last_meal >= data->t_die)
		{
			sem_wait(data->write);
			printf("%ld\t%d\t%s", live_time(data->t_start), philo->id, DEAD);
			sem_post(data->death);
			exit(1);
		}
		if (philo->meals_eaten >= data->n_eat && data->n_eat > 0)
		{
			sem_post(data->eat_limit);
			return ;
		}
	}
}

void	*monitor_meals(void *arg)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = (t_philo *)arg;
	while (i < philo->data->n_philo)
	{
		sem_wait(philo->data->eat_limit);
		i++;
	}
	sem_post(philo->data->death);
	return (NULL);
}

void	monitor_cleanup(t_philo *philo)
{
	int	i;

	i = 0;
	sem_wait(philo->data->death);
	while (i++ < philo->data->n_philo)
		sem_post(philo->data->eat_limit);
	i = 0;
	while (i < philo->data->n_philo)
	{
		kill(philo->pids[i], SIGKILL);
		i++;
	}
}
