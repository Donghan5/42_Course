/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:45:55 by donghank          #+#    #+#             */
/*   Updated: 2024/09/15 13:04:25 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	header(void)
{
	printf("\033[1;36m");
	printf("        ______________________\n");
	printf("       |  _________________  |\n");
	printf("       | |                 | |\n");
	printf("       | |  minishell      | |\n");
	printf("       | |                 | |\n");
	printf("       | |            42   | |\n");
	printf("       | |_________________| |\n");
	printf("       |  by pzinurov        |\n");
	printf("       |       donghank      |\n");
	printf("       |_____________________|\n");
	printf("\033[0m\n");
}
