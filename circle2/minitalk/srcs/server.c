/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:56:33 by donghank          #+#    #+#             */
/*   Updated: 2024/08/18 12:19:10 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	ft_handler(int signal)
{
	static int	bit;
	static char	tmp;

	if (signal == SIGUSR1)
		tmp |= (1 << bit);
	bit++;
	if (bit == 8)
	{
		ft_putchar_fd(tmp, 1);
		bit = 0;
		tmp = 0;
	}
}

/* the max pid value is inside of the /proc/sys/kernel/pid_max */
int	main(int ac, char **av)
{
	int	pid;

	(void)av;
	if (ac != 1)
		return (ft_printf("Please respect the format\n"), 0);
	pid = getpid();
	ft_printf("pid value: %d\n", pid);
	ft_printf("Waiting for a message\n");
	signal(SIGUSR1, ft_handler);
	signal(SIGUSR2, ft_handler);
	while (ac == 1)
		pause();
	return (0);
}
