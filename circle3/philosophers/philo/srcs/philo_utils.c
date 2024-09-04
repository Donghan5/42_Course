/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 11:43:38 by donghank          #+#    #+#             */
/*   Updated: 2024/09/04 13:46:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// to check the value of arg->finish by protecting mutex
int	monitoring_fini_mutex(t_arg *arg)
{
	pthread_mutex_lock(&(arg->fini_mutex));
	if (arg->finish == 1)
	{
		pthread_mutex_unlock(&(arg->fini_mutex));
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&(arg->fini_mutex));
		return (0);
	}
}

// to set arg->finish to 1 by protecting the mutex
void	pthread_mutex_lock_and_unlock(t_arg *arg)
{
	pthread_mutex_lock(&(arg->fini_mutex));
	arg->finish = 1;
	pthread_mutex_unlock(&(arg->fini_mutex));
}

// cut in short loop
void	ft_usleep(int ms)
{
	long int	time;

	time = get_time();
	while (get_time() - time < ms)
		usleep(ms / 10);
}
