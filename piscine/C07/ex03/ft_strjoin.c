/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 10:00:11 by donghank          #+#    #+#             */
/*   Updated: 2024/03/28 10:41:35 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

char	*ft_strcat(char *dest, char *src)
{
	char	*dest_start;

	dest_start = dest;
	while (*dest != '\0')
	{
		dest++;
	}
	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (dest_start);
}

int	total_size(int size, char **strs, char *sep)
{
	int	len_sep;
	int	len_strs;
	int	i;

	len_sep = ft_strlen(sep);
	len_strs = 0;
	i = 0;
	if (size == 1)
		return (ft_strlen(strs[0]) + 1);
	while (i < (size - 1))
	{
		len_strs += ft_strlen(strs[i]);
		len_strs += len_sep;
		i++;
	}
	len_strs += ft_strlen(strs[i]);
	return (len_strs + 1);
}

char	*ft_strjoin(int size, char **strs, char *sep)
{
	char	*str;
	int		i;
	int		total_len;

	if (size == 0)
	{
		str = (char *)malloc(1);
		*str = 0;
		return (str);
	}
	total_len = total_size(size, strs, sep);
	str = (char *)malloc(sizeof(char) * total_len);
	if (!(str))
		return (0);
	*str = 0;
	i = 0;
	while (i < size)
	{
		ft_strcat(str, strs[i]);
		if (i < (size - 1))
			ft_strcat(str, sep);
		i++;
	}
	return (str);
}
