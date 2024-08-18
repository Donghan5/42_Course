/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:56:24 by donghank          #+#    #+#             */
/*   Updated: 2024/08/18 13:04:38 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

/*
**	to check the server receve the signal sign
**	additional info : sigusr1 use to send the bits
**	sigusr2 use to receve the bit is arrived
**	I chose to use SIGUSR2 to check (receve)
*/
static void	ft_confirm(int signal)
{
	if (signal == SIGUSR2)
		ft_printf("Well receved the bits\n");
}

/* to send the level of the bits to server */
static void	ft_send_bits(pid_t pid, int i)
{
	int	bit;

	bit = 0;
	while (bit < 8)
	{
		if ((i & (0x01 << bit)) != 0)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(1000);
		bit++;
	}
}

int	main(int ac, char **av)
{
	pid_t				pid;
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
		if (pid < 0 || pid > 4194304)
			return (ft_printf("\033[0;31mInvailed PID value\033[0m\n"), 1);
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
