/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:07:23 by donghank          #+#    #+#             */
/*   Updated: 2024/08/31 17:07:24 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (printf("Invalid argument number\n"), 0);
	// if initialize is fail -> print error msg
	// if run thread of the philo is fail -> print error msg
}
