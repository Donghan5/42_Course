/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:56:24 by donghank          #+#    #+#             */
/*   Updated: 2024/08/08 18:02:18 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

/* to check the server receve the signal sign */
static void	ft_confirm(int signal)
{
	if (signal == SIGUSR1)
		ft_putstr_fd("Receve SIGUSR1\n", 1);
	else if (signal == SIGUSR2)
		ft_putstr_fd("Receve SIGUSR2\n", 1);
}

/* to send the level of the bits to conversation */
static void	ft_send_bits(int pid, int i)
{
	int	bit;

	bit = 0;
	while (bit < 8)
	{
		if ((i & (0x01 << bit)) != 0)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(200);
		bit++;
	}
}

int	main(int ac, char **av)
{
	int					pid;
	int					i;
	struct sigaction	sa;

	i = 0;
	if (ac == 3)
	{
		sa.sa_handler = ft_confirm;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGUSR1, &sa, NULL);
		sigaction(SIGUSR2, &sa, NULL);
		pid = ft_atoi(av[1]);
		while (av[2][i] != '\0')
		{
			ft_send_bits(pid, av[2][i]);
			i++;
		}
		ft_send_bits(pid, '\n');
	}
	else
		return (ft_printf("Please make sure format\n"), 1);
	return (0);
}
