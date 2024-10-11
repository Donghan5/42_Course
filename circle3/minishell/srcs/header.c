/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:45:55 by donghank          #+#    #+#             */
/*   Updated: 2024/10/11 23:20:40 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// when start the minishell, print the header
void	header(void)
{
	printf("\033[1;36m");
	printf("              _       _      __         ____\n");
	printf("   ____ ___  (_)___  (_)____/ /_  ___  / / /\n");
	printf("  / __ `__ \\/ / __ \\/ / ___/ __ \\/ _ \\/ / /\n");
	printf(" / / / / / / / / / / (__  ) / / /  __/ / /\n");
	printf("/_/ /_/ /_/_/_/ /_/_/____/_/ /_/\\___/_/_/\n");
	printf("                   by donghank pzinurov\n");
	printf("\033[0m\n");
}
