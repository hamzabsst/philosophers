/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:41:26 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/24 02:11:50 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

# define DIED  "is dead 💀"
# define EAT  "is eating  🍔"
# define SLP  "is sleeping 😴"
# define R  "has taken a right fork 🍴"
# define L  "has taken a left fork 🍴"
# define THNK  "is thinking 🤔"
# define FINISH  "simulation is finished"

typedef struct s_data
{
	int				philos;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	long 			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	int				end;
}	t_data;

typedef struct s_philo
{
	int			id;
	int			meals_eaten;
	long		last_meal;
	pthread_t	thread;
	t_data		*data;
}	t_philo;

int		parsing(int ac, char **av, t_data *data, t_philo **philo);
void	*routine(void *arg);
long	get_time(void);
void	*monitor_death(void *arg);

#endif
