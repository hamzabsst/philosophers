/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:41:26 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 07:57:09 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <semaphore.h>

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

# define FORK  "has taken a fork\n"
# define EAT  "is eating\n"
# define SLEEP  "is sleeping\n"
# define THINK  "is thinking\n"
# define DEAD  "died\n"

typedef struct s_data
{
	int		philo;
	int		t_die;
	int		t_eat;
	int		t_sleep;
	int		n_eat;
	long	t_start;
	sem_t	*forks;
	sem_t	*write;
	sem_t	*death;
	sem_t	*eat_limit;
	sem_t	*meal_mutex;
}	t_data;

typedef struct s_philo
{
	pthread_t	thread;
	int			id;
	int			meals_eaten;
	long		last_meal;
	pid_t		*pids;
	t_data		*data;
}	t_philo;

int		init_data(t_data *data, int ac, char **av);
void	create_philos(t_data *data, t_philo *philo);
void	monitoring(t_philo *philo);
void	*second_monitor(void *arg);
void	monitore(t_philo *philo);
void	*routine(void *arg);
long	live_time(long start);
void	cleanup(t_data *data, t_philo *philo);
void	print_msg(t_philo *philo, char *msg, int release_sem);
void	sem_unlinking(void);

#endif
