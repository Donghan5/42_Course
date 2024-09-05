/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:07:30 by donghank          #+#    #+#             */
/*   Updated: 2024/09/05 14:14:13 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// start with odd number philosopher. To avoid state deadlock
// So start with left fork (I'm using Dijkstra's solution)
// using with odd number
int	philo_action_eat(t_arg *arg, t_philo *philo)
{
	pthread_mutex_lock(&(arg->fork[philo->left]));
	philo_stat_print(arg, philo->id, "has taken a fork");
	if (arg->num_of_philo != 1)
	{
		pthread_mutex_lock(&(arg->fork[philo->right]));
		philo_stat_print(arg, philo->id, "has taken a fork");
		philo_stat_print(arg, philo->id, "is eating");
		pthread_mutex_lock(&(arg->time));
		philo->last_eat_time = get_time();
		pthread_mutex_unlock(&(arg->time));
		philo->eat_count++;
		passing_time((long long)arg->time_to_eat, arg);
		pthread_mutex_unlock(&(arg->fork[philo->right]));
	}
	pthread_mutex_unlock(&(arg->fork[philo->left]));
	return (0);
}

static void	philo_lifespan(t_arg *arg, t_philo *philo)
{
	if (arg->num_of_philo - 1 == philo->id && philo->eat_count == 0)
		usleep(1);
	philo_action_eat(arg, philo);
	if (arg->num_of_philo == 1)
		passing_time((long long)arg->time_to_sleep, arg);
}

// start with odd number (index) philosopher
// if the eat_times and eat_count -> exit the loop
// separate the function eating // sleeping and thinking
void	*philo_thread(void *argv)
{
	t_arg	*arg;
	t_philo	*philo;

	philo = (t_philo *)argv;
	arg = philo->arg;
	if (philo->id % 2 == 0)
		time_thinking(arg);
	while (!monitoring_fini_mutex(arg))
	{
		philo_lifespan(arg, philo);
		if (arg->eat_times == philo->eat_count)
		{
			arg->finished_eat++;
			break ;
		}
		philo_stat_print(arg, philo->id, "is sleeping");
		passing_time((long long)arg->time_to_sleep, arg);
		philo_stat_print(arg, philo->id, "is thinking");
	}
	return (0);
}

// to print the status of the philos
// now = to calculate the passed time (from start point)
int	philo_stat_print(t_arg *arg, int id, char *msg)
{
	long long	cur_time;

	pthread_mutex_lock(&(arg->print));
	cur_time = get_time();
	if (cur_time < 0)
		return (-1);
	if (!monitoring_fini_mutex(arg))
		printf("%lld %d %s\n", cur_time - arg->start_time, id, msg);
	if (ft_strncmp(msg, "died", 4) == 0)
		return (0);
	pthread_mutex_unlock(&(arg->print));
	return (0);
}

// monitoring death each of the philos(thread)
// cond 1. eat_times not 0, fini_eat(in philo_thread) == num philo
// cond 2. each thread of philo(cur - last) is longer than time of died
void	monitoring(t_arg *arg, t_philo *philo)
{
	int			i;
	long long	cur_time;

	while (!monitoring_fini_mutex(arg))
	{
		if ((arg->eat_times != 0) && (arg->num_of_philo == arg->finished_eat))
			pthread_mutex_lock_and_unlock(arg);
		i = 0;
		while (i < arg->num_of_philo)
		{
			cur_time = get_time();
			if ((cur_time - philo[i].last_eat_time) >= (arg->time_to_die))
			{
				philo_stat_print(arg, i, "died");
				pthread_mutex_lock_and_unlock(arg);
				pthread_mutex_unlock(&(arg->print));
			}
			i++;
		}
	}
}
