/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 06:27:39 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 09:48:37 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	monitor_meals(t_philo *philo)
{
	int		i;
	int		done_eating;
	t_data	*data;

	i = 0;
	done_eating = 0;
	data = philo->data;
	while (i < data->n_philo)
	{
		pthread_mutex_lock(&philo[i].meal_mutex);
		if (philo[i].meals_eaten >= data->n_eat && data->n_eat > 0)
			done_eating++;
		pthread_mutex_unlock(&philo[i].meal_mutex);
		i++;
	}
	if (done_eating == data->n_philo)
		return (1);
	return (0);
}

static void	monitor_death(t_philo *philo, int i)
{
	long	curr_time;

	curr_time = live_time(philo->data->t_start);
	pthread_mutex_lock(&philo[i].meal_mutex);
	if (curr_time - philo[i].last_meal >= philo->data->t_die)
	{
		pthread_mutex_unlock(&philo[i].meal_mutex);
		print_msg(&philo[i], DEAD);
		pthread_mutex_lock(&philo->data->end_mutex);
		philo->data->end = 1;
		pthread_mutex_unlock(&philo->data->end_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo[i].meal_mutex);
}

void	monitor(t_philo *philo)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < philo->data->n_philo)
		{
			monitor_death(philo, i);
			pthread_mutex_lock(&philo->data->end_mutex);
			if (philo->data->end == 1)
			{
				pthread_mutex_unlock(&philo->data->end_mutex);
				return ;
			}
			pthread_mutex_unlock(&philo->data->end_mutex);
			i++;
		}
		if (philo->data->n_eat > 0 && monitor_meals(philo))
		{
			pthread_mutex_lock(&philo->data->end_mutex);
			philo->data->end = 1;
			pthread_mutex_unlock(&philo->data->end_mutex);
			return ;
		}
	}
}
