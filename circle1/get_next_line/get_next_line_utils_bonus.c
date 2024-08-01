/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils _bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:29:32 by donghank          #+#    #+#             */
/*   Updated: 2024/06/11 13:06:47 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*gnl_strchr(char *s, int c)
{
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (&s[i]);
		i++;
	}
	if (c == '\0')
		return (&s[i]);
	return (NULL);
}

size_t	gnl_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*gnl_strjoin(char *s1, char *s2)
{
	char	*new_s;
	char	*tmp;
	size_t	total_len;

	if (!s1 && !s2)
		return (NULL);
	total_len = gnl_strlen(s1) + gnl_strlen(s2);
	new_s = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!new_s)
		return (NULL);
	tmp = new_s;
	if (s1)
	{
		while (*s1)
			*tmp++ = *s1++;
	}
	if (s2)
	{
		while (*s2)
			*tmp++ = *s2++;
	}
	*tmp = '\0';
	return (new_s);
}

char	*gnl_substr(char *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= gnl_strlen(s))
	{
		ret = (char *)malloc(1);
		if (!ret)
			return (NULL);
		ret[0] = '\0';
		return (ret);
	}
	if (len > gnl_strlen(s) - start)
		len = gnl_strlen(s) - start;
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (i < len)
		ret[i++] = s[start++];
	ret[i] = '\0';
	return (ret);
}

char	*gnl_strdup(char *s)
{
	char	*new_s;
	size_t	len;
	size_t	index;

	if (!s)
		return (NULL);
	len = gnl_strlen(s);
	new_s = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_s)
		return (NULL);
	index = 0;
	while (index < len)
	{
		new_s[index] = s[index];
		index++;
	}
	new_s[index] = '\0';
	return (new_s);
}
