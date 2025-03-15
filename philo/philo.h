/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:41:26 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/15 21:36:08 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <signal.h>
# include <pthread.h>
# include <semaphore.h>
# include <limits.h>

typedef struct s_data
{
	int				philos;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
}	t_data;

typedef struct s_philo
{
	int			id;
	int			meals_eaten;
	long long	last_meal;
	pthread_t	thread;
	t_data		*data;
}	t_philo;

int	parsing(int ac, char **av, t_data *data);

#endif
