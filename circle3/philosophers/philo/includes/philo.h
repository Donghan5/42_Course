/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:09:02 by donghank          #+#    #+#             */
/*   Updated: 2024/09/04 13:46:01 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*==================== header =====================*/
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <memory.h>
# include <sys/time.h>

/*==================== structs =====================*/
// all of the arguement line
// num_of_philo to eat_times = arguement values (assignement of subject)
// fork = to protect the forks
// print = print msg (by philosophers)
// time = to trace the time
typedef struct s_arg
{
	int				num_of_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				eat_times;
	int				finish;
	int				finished_eat;
	long long		start_time;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print;
	pthread_mutex_t	time;
	pthread_mutex_t	fini_mutex;
}	t_arg;

// all of the element of the philo
// id = id of the philo
// left = left fork || right = right fork
// last_eat_time = last eat of each philo
// eat_conunt = to calculate times of eat
typedef struct s_philo
{
	t_arg		*arg;
	pthread_t	thread;
	int			id;
	int			left;
	int			right;
	long long	last_eat_time;
	int			eat_count;
}	t_philo;

/*==================== utils.c =====================*/
int			ft_strncmp(char *s1, char*s2, int n);
int			ft_atoi(char *str);
void		passing_time(long long wait_time, t_arg *arg);
void		time_thinking(t_arg *arg);
long long	get_time(void);

/*==================== philo_utils.c =====================*/
int			monitoring_fini_mutex(t_arg *arg);
void		pthread_mutex_lock_and_unlock(t_arg *arg);
void		ft_usleep(int ms);

/*==================== init.c =====================*/
int			init_mutex_element(t_arg *arg);
int			init_args_element(t_arg *arg, int ac, char **av);
int			init_philo(t_philo **philo, t_arg *arg);

/*==================== philo.c =====================*/
int			create_philo_thread(t_arg *arg, t_philo *philo);
int			philo_action_eat(t_arg *arg, t_philo *philo);
void		*philo_thread(void *argv);
int			philo_stat_print(t_arg *arg, int id, char *msg);
void		monitoring(t_arg *arg, t_philo *philo);

/*==================== main.c =====================*/
void		distroy_all_mutex(t_arg *arg);

#endif
