/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:56:33 by donghank          #+#    #+#             */
/*   Updated: 2024/08/17 15:04:12 by donghank         ###   ########.fr       */
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

/*
1) Verify that the limitations of PID is working
(PID must be more than 0 and there is also the maximum PID allowed for UNIX - find the value). I crashed the program while using PID = -1.
2) From my prospective, the part of server acknowledgement is done not very correctly as printing the signals isn't very informative for the end user who launches the client. In the subvject it is written that the server acknowledges the MESSAGES so I suppose that we need to send the signal to client only when we stop printing the message on server side. But you can try to defense your solution if you provide the logical explanation of your code =)
3) Perhaps there is an option to accelerate the printing logic for server - for 100 characters, it is printing not as fast as the creators of the projects must have thought about this. But it's just an option here.
 */
