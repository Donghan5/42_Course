/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:56:24 by donghank          #+#    #+#             */
/*   Updated: 2024/08/07 13:40:01 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

/* to check the server receve the signal sign */
void	ft_oksignal(int signal)
{
	if (signal == SIGUSR1)
		ft_printf("Receve SIGUSR1\n");
	else
		ft_printf("Receve SIGUSR2\n");
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
		usleep(100);
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
		sa.sa_handler = ft_oksignal;
		sa.sa_flags = SA_SIGINFO;
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
