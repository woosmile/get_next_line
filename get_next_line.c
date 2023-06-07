/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 17:40:40 by woosekim          #+#    #+#             */
/*   Updated: 2023/01/25 16:03:10 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	add_string(char **save, char *line)
{
	char	*temp;

	if (!*save)
		temp = ft_strdup(line);
	else
		temp = ft_strjoin(*save, line);
	if (!temp)
		return (0);
	else
	{
		*save = temp;
		return (1);
	}
}

void	read_string(int fd, char **save, char *line, ssize_t *read_result)
{
	int		i;

	*read_result = read(fd, line, BUFFER_SIZE);
	while (*read_result != 0)
	{
		if (*read_result == -1)
			return ;
		line[*read_result] = 0;
		if (!add_string(save, line))
		{
			free_memory(save);
			return ;
		}
		i = 0;
		while (line[i] != 0)
		{
			if (line[i] == '\n')
				return ;
			i++;
		}
		*read_result = read(fd, line, BUFFER_SIZE);
	}
}

void	input_string(char **save, char **line)
{
	int		len;
	int		i;

	len = 0;
	while (save[0][len] != '\n' && save[0][len] != 0)
		len++;
	if (save[0][len] == '\n')
		len = len + 2;
	else if (save[0][len] == 0)
		len = len + 1;
	*line = (char *)malloc(sizeof(char) * len);
	if (!*line)
	{
		free_memory(save);
		return ;
	}
	i = 0;
	while (i < len)
	{
		line[0][i] = save[0][i];
		i++;
	}
	line[0][len - 1] = 0;
}

void	next_string(char **save, char **line)
{
	size_t	pre_len;
	char	*new;

	pre_len = ft_strlen(*line);
	if (save[0][pre_len] != 0)
	{
		new = ft_strdup(save[0] + pre_len);
		if (!new)
		{
			free_memory(save);
			free_memory(line);
			return ;
		}
		free_memory(save);
		*save = new;
	}
	else
		**save = 0;
}

char	*get_next_line(int fd)
{
	static char	*save;
	char		new[BUFFER_SIZE + 1];
	char		*line;
	ssize_t		read_result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	read_string(fd, &save, new, &read_result);
	if (read_result == -1 || !save || !*save)
	{
		free_memory(&save);
		return (0);
	}
	input_string(&save, &line);
	if (!line)
		return (0);
	next_string(&save, &line);
	if (!save)
		return (0);
	return (line);
}
