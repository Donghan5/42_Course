/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcapitalize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:38:15 by donghank          #+#    #+#             */
/*   Updated: 2024/03/18 18:11:36 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

char	*ft_strcapitalize(char *str)
{
	int	new;
	int	i;

	i = 0;
	new = 1;
	while (str[i] != '\0')
	{
		if (new == 1 && (str[i] >= 97 && str[i] <= 122))
			str[i] = str[i] - 32;
		else if (new == 0 && (str[i] >= 65 && str[i] <= 90))
			str[i] = str[i] + 32;
		if (str[i] > 47 && str[i] < 58)
			new = 0;
		else if ((str[i] > 64 && str[i] < 91) || (str[i] > 96 && str[i] < 123))
			new = 0;
		else
			new = 1;
		i++;
	}
	return (str);
}
