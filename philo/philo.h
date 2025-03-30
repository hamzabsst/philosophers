/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:41:26 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/30 08:10:42 by hbousset         ###   ########.fr       */
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

# define DEAD  "is dead 💀\n"
# define EAT  "is eating 🍽️\n"
# define SLEEP  "is sleeping 😴\n"
# define R_FORK  "has taken the right fork 🍴\n"
# define L_FORK  "has taken the left fork 🍴\n"
# define THINK  "is thinking 🤔\n"
# define FINISH  "simulation is finished ✅\n"

typedef struct s_data
{
	int				philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	long			t_start;
	int				end;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	end_mutex;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;
	pthread_mutex_t	meal_mutex;
	pthread_t		thread;
	t_data			*data;
}	t_philo;

typedef struct s_monitor
{
	t_philo	*philos;
	t_data	*data;
	long	current_t;
	long	t_since_meal;
	long	min_t_remain;
	int		i;
	int		all_done;
}	t_monitor;

int		init_data(t_data *data, int ac, char **av);
int		create_philo(t_data *data, t_philo *philo);
long	live_time(long start);
void	*one_philo(void *arg);
void	*routine(void *arg);
void	*monitor_death(void *arg);
void	cleanup(t_data *data, t_philo *philo);
void	smart_sleep(long duration, t_data *data);
long	live_time(long start);
void	print_msg(t_philo *philo, char *msg);

#endif
