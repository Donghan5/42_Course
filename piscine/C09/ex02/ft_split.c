/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 14:27:35 by donghank          #+#    #+#             */
/*   Updated: 2024/04/01 17:52:42 by donghank         ###   ########.fr       */
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

int	is_sep(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i] != '\0')
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

int	word_count(char *str, char *charset)
{
	int	count;

	count = 0;
	while (*str != '\0')
	{
		if (!is_sep(*str, charset))
		{
			count++;
			while (!is_sep(*str, charset) && *str != '\0')
				str++;
		}
		else
			str++;
	}
	return (count);
}

char	*ft_strncpy(char *dest, char *src, int n)
{
	int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	**ft_split(char *str, char *charset)
{
	char	**strs;
	int		i;
	int		j;
	int		count;

	count = word_count(str, charset);
	strs = (char **)malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (*str != '\0' && i < count)
	{
		while (*str != '\0' && is_sep(*str, charset))
			str++;
		j = 0;
		while (str[j] != '\0' && !is_sep(str[j], charset))
			j++;
		if (j > 0)
		{
			strs[i] = (char *)malloc(sizeof(char) * (j + 1));
			ft_strncpy(strs[i++], str, j)[j] = '\0';
			str += j;
		}
	}
	strs[i] = 0;
	return (strs);
}
