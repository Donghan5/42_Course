/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:07:15 by donghank          #+#    #+#             */
/*   Updated: 2024/09/03 18:06:45 by donghank         ###   ########.fr       */
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
	if (pthread_mutex_init(&(arg->fini_mutex), NULL))
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
		if (arg->num_of_philo == 1)
			arg->eat_times = 0;
	}
	if (init_mutex_element(arg))
		return (1);
	return (0);
}

// init_philo --> make multiple philos
// i -> create the number of the philos
int	init_philo(t_philo **philo, t_arg *arg)
{
	int	i;

	i = 0;
	*philo = malloc(sizeof(t_philo) * arg->num_of_philo);
	if (!(*philo))
		return (1);
	while (i < arg->num_of_philo)
	{
		(*philo)[i].arg = arg;
		(*philo)[i].id = i;
		(*philo)[i].left = i;
		(*philo)[i].right = (i + 1) % arg->num_of_philo;
		(*philo)[i].last_eat_time = get_time();
		(*philo)[i].eat_count = 0;
		i++;
	}
	return (0);
}

// create multiple philo thread
int	create_philo_thread(t_arg *arg, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < arg->num_of_philo)
	{
		philo[i].last_eat_time = get_time();
		if (pthread_create(&(philo[i].thread), NULL, philo_thread, &(philo[i])))
			return (1);
		i++;
	}
	monitoring(arg, philo);
	i = 0;
	while (i < arg->num_of_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	return (0);
}
