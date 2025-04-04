/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 07:57:21 by hbousset          #+#    #+#             */
/*   Updated: 2025/04/04 07:57:37 by hbousset         ###   ########.fr       */
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
	data->philo = temp;
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
	data->forks = sem_open("/forks", O_CREAT, 0644, data->philo);
	data->write = sem_open("/write", O_CREAT, 0644, 1);
	data->death = sem_open("/death", O_CREAT, 0644, 0);
	data->eat_limit = sem_open("/limit", O_CREAT, 0644, 0);
	data->meal_mutex = sem_open("/meal_mutex", O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED || data->write == SEM_FAILED
		|| data->death == SEM_FAILED || data->eat_limit == SEM_FAILED
		|| data->meal_mutex == SEM_FAILED)
		return (printf("Error: sem_open failed\n"), 1);
	return (0);
}

void	create(t_philo *philo)
{
	philo->last_meal = live_time(philo->data->t_start);
	pthread_create(&philo->thread, NULL, &routine, philo);
	monitoring(philo);
	pthread_join(philo->thread, NULL);
	return ;
}

void	create_philos(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	philo->data = data;
	while (i < data->philo)
	{
		philo->pids[i] = fork();
		if (philo->pids[i] < 0)
		{
			while (--i >= 0)
				kill(philo->pids[i], SIGKILL);
			printf("Error: fork failed\n");
			return;
		}
		else if (philo->pids[i] == 0)
		{
			philo->id = i + 1;
			philo->meals_eaten = 0;
			philo->last_meal = live_time(data->t_start);
			create(philo);
			exit(0);
		}
		i++;
	}
	if (data->philo == 1)
	{
		waitpid(philo->pids[0], NULL, 0);
		return;
	}
	pthread_create(&philo->thread, NULL, &second_monitor, philo);
	monitore(philo);
	pthread_join(philo->thread, NULL);
}
