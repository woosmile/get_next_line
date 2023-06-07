/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:42:44 by woosekim          #+#    #+#             */
/*   Updated: 2023/01/23 17:35:28 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	free_memory(char **mem)
{
	if (*mem != 0)
	{
		free(*mem);
		*mem = 0;
	}
}

size_t	ft_strlen(char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != 0)
		len++;
	return (len);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char				*str;
	size_t				len1;
	size_t				len2;
	unsigned long long	i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	i = 0;
	str = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (!str)
		return (0);
	while (s1[i] != 0)
	{
		str[i] = s1[i];
		i++;
	}
	while (*s2 != 0)
	{
		str[i++] = *s2;
		s2++;
	}
	str[i] = 0;
	free(s1);
	return (str);
}

char	*ft_strdup(char *s1)
{
	char		*arr;
	size_t		len;
	size_t		i;

	len = ft_strlen(s1);
	i = 0;
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (!arr)
		return (0);
	while (i < len)
	{
		arr[i] = s1[i];
		i++;
	}
	arr[i] = 0;
	return (arr);
}
