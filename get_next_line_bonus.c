/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 17:40:40 by woosekim          #+#    #+#             */
/*   Updated: 2023/01/25 16:04:07 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	check_fd(int fd, t_list **head, t_list *head_temp, t_list **node)
{
	t_list	*new;

	if (!*head)
	{
		*head = ft_lstnew(fd, 0);
		*node = *head;
	}
	else
	{
		while ((*head)->next != 0)
		{
			if ((*head)->fd == fd)
				break ;
			*head = (*head)->next;
		}
		if ((*head)->fd != fd)
		{
			new = ft_lstnew(fd, *head);
			(*head)->next = new;
			*head = (*head)->next;
		}
		*node = *head;
		*head = head_temp;
	}
}

void	read_string(int fd, t_list **node, char *buffer, ssize_t *read_len)
{
	int		i;
	char	*temp;

	*read_len = read(fd, buffer, BUFFER_SIZE);
	while (*read_len != 0)
	{
		if (*read_len == -1)
			return ;
		buffer[*read_len] = 0;
		temp = ft_strjoin((*node)->str, buffer);
		if (!temp)
		{
			*read_len = -1;
			return ;
		}
		(*node)->str = temp;
		i = 0;
		while (buffer[i] != 0)
		{
			if (buffer[i] == '\n')
				return ;
			i++;
		}
		*read_len = read(fd, buffer, BUFFER_SIZE);
	}
}

void	input_string(t_list **head, t_list	**node, char **line)
{
	int		len;
	int		i;

	len = 0;
	while (*((*node)->str + len) != '\n' && *((*node)->str + len) != 0)
		len++;
	if (*((*node)->str + len) == '\n')
		len = len + 2;
	else if (*((*node)->str + len) == 0)
		len = len + 1;
	*line = (char *)malloc(sizeof(char) * len);
	if (!*line)
	{
		clear_node(head, node, 0);
		return ;
	}
	i = 0;
	while (i < len)
	{
		line[0][i] = *((*node)->str + i);
		i++;
	}
	line[0][len - 1] = 0;
}

void	next_string(t_list **head, t_list **node, char *line)
{
	size_t	pre_len;
	char	*new;

	pre_len = ft_strlen(line);
	if (*((*node)->str + pre_len) != 0)
	{
		new = ft_strdup((*node)->str + pre_len);
		if (!new)
		{
			clear_node(head, node, 0);
			free(line);
			line = 0;
			return ;
		}
		free((*node)->str);
		(*node)->str = new;
	}
	else
		*((*node)->str) = 0;
}

char	*get_next_line(int fd)
{
	static t_list	*head;
	t_list			*node;
	char			*line;
	ssize_t			read_len;
	char			buffer[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	check_fd(fd, &head, head, &node);
	if (!node)
		return (0);
	read_string(fd, &node, buffer, &read_len);
	if (read_len == -1 || !*(node->str))
	{
		clear_node(&head, &node, 0);
		return (0);
	}
	input_string(&head, &node, &line);
	if (!line)
		return (0);
	next_string(&head, &node, line);
	if (!node)
		return (0);
	return (line);
}
