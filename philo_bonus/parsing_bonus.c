/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:09:23 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 11:02:08 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_atoi(const char *str, int *result)
{
	long	nbr;
	int		i;

	nbr = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] < '0' || str[i] > '9')
		return (1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (nbr > (INT_MAX - (str[i] - '0')) / 10)
			return (1);
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (1);
	*result = (int)nbr;
	return (0);
}

static int	init_variables(int ac, char **av, t_data *data)
{
	int	temp;

	if (ft_atoi(av[1], &temp) || temp <= 0)
		return (printf("Error: Invalid number of philosophers\n"), 1);
	data->n_philo = temp;
	if (ft_atoi(av[2], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_die\n"), 1);
	data->t_die = temp;
	if (ft_atoi(av[3], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_eat\n"), 1);
	data->t_eat = temp;
	if (ft_atoi(av[4], &temp) || temp <= 0)
		return (printf("Error: Invalid time_to_sleep\n"), 1);
	data->t_sleep = temp;
	if (ac == 6)
	{
		if (ft_atoi(av[5], &temp) || temp <= 0)
			return (printf("Error: Invalid times_each_philos_must_eat\n"), 1);
		data->n_eat = temp;
	}
	else
		data->n_eat = -1;
	data->t_start = live_time(0);
	return (0);
}

int	init_data(t_data *data, int ac, char **av)
{
	if (init_variables(ac, av, data))
		return (1);
	sem_unlinking();
	data->forks = sem_open("/forks", O_CREAT, 0644, data->n_philo);
	data->write = sem_open("/write", O_CREAT, 0644, 1);
	data->death = sem_open("/death", O_CREAT, 0644, 0);
	data->eat_limit = sem_open("/limit", O_CREAT, 0644, 0);
	data->queue = sem_open("/queue", O_CREAT, 0644, data->n_philo / 2);
	if (data->forks == SEM_FAILED || data->write == SEM_FAILED
		|| data->death == SEM_FAILED || data->eat_limit == SEM_FAILED
		|| data->queue == SEM_FAILED)
		return (printf("Error: sem_open failed\n"), 1);
	return (0);
}

static void	spawn_philos(t_data *data, t_philo *philo)
{
	int	i;

	i = -1;
	philo->data->t_start = live_time(0);
	while (++i < data->n_philo)
	{
		philo->pids[i] = fork();
		if (philo->pids[i] == 0)
		{
			philo->id = i + 1;
			philo->data->t_start = philo->data->t_start;
			philo->last_meal = live_time(philo->data->t_start);
			if (philo->data->n_philo == 1)
			{
				routine(philo);
				cleanup(philo->data, philo);
				exit(0);
			}
			pthread_create(&philo->thread, NULL, &routine, philo);
			monitor(philo);
			pthread_join(philo->thread, NULL);
			sem_closing(philo->data);
			exit(0);
		}
	}
}

void	create_philos(t_data *data, t_philo *philo)
{
	int	i;

	philo->data = data;
	philo->meals_eaten = 0;
	philo->thread = 0;
	philo->id = 0;
	philo->last_meal = 0;
	philo->data->t_start = 0;
	spawn_philos(data, philo);
	if (data->n_philo == 1)
	{
		waitpid(philo->pids[0], NULL, 0);
		return ;
	}
	pthread_create(&philo->thread, NULL, &monitor_meals, philo);
	monitor_cleanup(philo);
	pthread_join(philo->thread, NULL);
	i = 0;
	while (i < data->n_philo)
	{
		waitpid(philo->pids[i], NULL, 0);
		i++;
	}
}
