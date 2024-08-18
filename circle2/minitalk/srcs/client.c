/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:56:41 by donghank          #+#    #+#             */
/*   Updated: 2024/08/18 13:04:50 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/* to send the bits
** Sigusr1 = 1
** Sigusr2 = 0
*/
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
	pid_t	pid;
	int		i;

	i = 0;
	if (ac == 3)
	{
		pid = ft_atoi(av[1]);
		if (pid < 0 || pid > 4194304)
			return (ft_printf("\033[0;31mInvailed PID value\033[0m\n"), 1);
		while (av[2][i] != '\0')
		{
			ft_send_bits(pid, av[2][i]);
			i++;
		}
		ft_send_bits(pid, '\n');
		ft_send_bits(pid, '\0');
	}
	else
		ft_printf("\033[0;31mWrong format: ./exec pid msg\033[0m\n");
	return (0);
}
