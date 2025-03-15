/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 02:37:59 by hbousset          #+#    #+#             */
/*   Updated: 2025/03/15 21:40:22 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (parsing(ac, av, &data))
		return (1);
	printf("Philosophers: %d\n", data.philos);
	printf("Time to die: %d ms\n", data.t_die);
	printf("Time to eat: %d ms\n", data.t_eat);
	printf("Time to sleep: %d ms\n", data.t_sleep);
	if (data.n_eat != -1)
		printf("Each must eat: %d times\n", data.n_eat);
	else
		printf("No meal limit\n");
	return (0);
}
