/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:07:15 by donghank          #+#    #+#             */
/*   Updated: 2024/09/01 17:00:54 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// int init_mutex fail -> return 1
int	init_mutex_element(t_arg *arg)
{
	int	idx;

	if (pthread_mutex_init(&(arg->print), NULL))
		return (1);
	if (pthread_mutex_init(&(arg->time), NULL))
		return (1);
	arg->fork = malloc(sizeof(pthread_mutex_t) * arg->num_of_philo);
	if (!arg->fork)
		return (1);
	idx = 0;
	while (idx < arg->num_of_philo)
	{
		if (pthread_mutex_init(&arg->fork[idx], NULL))
			return (1);
		idx++;
	}
	return (0);
}

// init all element in the struct arg
int	init_args_element(t_arg *arg, int ac, char **av)
{
	arg->num_of_philo = ft_atoi(av[1]);
	arg->time_to_die = ft_atoi(av[2]);
	arg->time_to_eat = ft_atoi(av[3]);
	arg->time_to_sleep = ft_atoi(av[4]);
	arg->start_time = get_time();
	if (arg->num_of_philo <= 0 || arg->time_to_die < 0 \
	|| arg->time_to_eat < 0 || arg->time_to_sleep < 0 || \
	arg->start_time < 0)
		return (1);
	if (ac == 6)
	{
		arg->eat_times = ft_atoi(av[5]);
		if (arg->eat_times <= 0)
			return (1);
	}
	arg->finish = 0;
	arg->finished_eat = 0;
	if (init_mutex_element(arg))
		return (1);
	return (0);
}

// init_philo --> make multiple philos
// i -> create the number of the philos
int	init_philo(t_philo **philo, t_arg *arg)
{
	int	idx;

	idx = 0;
	*philo = malloc(sizeof(t_philo) * arg->num_of_philo);
	if (!(*philo))
		return (1);
	while (idx < arg->num_of_philo)
	{
		(*philo)[idx].arg = arg;
		(*philo)[idx].id = idx;
		(*philo)[idx].left = idx;
		(*philo)[idx].right = (idx + 1) % arg->num_of_philo;
		(*philo)[idx].last_eat_time = get_time();
		(*philo)[idx].eat_count = 0;
		idx++;
	}
	return (0);
}
