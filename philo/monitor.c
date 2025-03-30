/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 06:27:39 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/30 08:39:16 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	handle_death(t_philo *philo, t_data *data, long current_t, int i)
{
	pthread_mutex_lock(&data->end_mutex);
	data->end = 1;
	pthread_mutex_unlock(&data->end_mutex);
	pthread_mutex_lock(&data->write_lock);
	printf("%ld ms: [%d] has died \n", current_t, i + 1);
	pthread_mutex_unlock(&data->write_lock);
	pthread_mutex_unlock(&philo->meal_mutex);
}

static void	update_monitor(t_monitor *mon)
{
	mon->current_t = live_time(mon->data->t_start);
	mon->all_done = 1;
	mon->min_t_remain = mon->data->t_die;
	mon->i = 0;
}

static int	check_philosopher(t_monitor *mon)
{
	pthread_mutex_lock(&mon->philos[mon->i].meal_mutex);
	mon->t_since_meal = mon->current_t - mon->philos[mon->i].last_meal;
	if (mon->data->t_die - mon->t_since_meal < mon->min_t_remain)
		mon->min_t_remain = mon->data->t_die - mon->t_since_meal;
	if (mon->t_since_meal >= mon->data->t_die)
	{
		handle_death(&mon->philos[mon->i], mon->data, mon->current_t, mon->i);
		return (1);
	}
	if (mon->data->n_eat != -1)
	{
		if (mon->philos[mon->i].meals_eaten < mon->data->n_eat)
			mon->all_done = 0;
	}
	pthread_mutex_unlock(&mon->philos[mon->i].meal_mutex);
	return (0);
}

static void	handle_simulation_end(t_monitor *mon)
{
	pthread_mutex_lock(&mon->data->end_mutex);
	mon->data->end = 1;
	pthread_mutex_unlock(&mon->data->end_mutex);
	pthread_mutex_lock(&mon->data->write_lock);
	printf("%ld ms: %s", live_time(mon->data->t_start), FINISH);
	pthread_mutex_unlock(&mon->data->write_lock);
}

void	*monitor_death(void *arg)
{
	t_monitor	mon;

	mon.philos = (t_philo *)arg;
	mon.data = mon.philos[0].data;
	while (1)
	{
		update_monitor(&mon);
		while (mon.i < mon.data->philo)
		{
			if (check_philosopher(&mon))
				return (NULL);
			mon.i++;
		}
		if (mon.data->n_eat != -1 && mon.all_done)
		{
			handle_simulation_end(&mon);
			return (NULL);
		}
		if (mon.min_t_remain > 0)
			usleep(mon.min_t_remain * 1000);
		else
			usleep(1000);
	}
}
