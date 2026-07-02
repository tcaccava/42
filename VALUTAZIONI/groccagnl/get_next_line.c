/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grocca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 15:36:24 by grocca            #+#    #+#             */
/*   Updated: 2026/06/17 15:36:30 by grocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list	*new_node(int fd)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->fd = fd;
	node->buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!node->buf)
	{
		free(node);
		return (NULL);
	}
	(node->buf)[0] = '\0';
	return (node);
}

t_list	*list(t_list *lst, int fd)
{
	t_list	*start;

	start = lst;
	if (lst)
	{
		if (fd == lst->fd)
			return (lst);
		while (lst->next != start)
		{
			lst = lst->next;
			if (fd == lst->fd)
				return (lst);
		}
		lst = new_node(fd);
	}
	else
		lst = new_node(fd);
	if (start && lst)
	{
		lst->next = start->next;
		start->next = lst;
	}
	else if (!start && lst)
		lst->next = lst;
	return (lst);
}

char	*free_list(t_list *lst, int fd)
{
	t_list	*node;

	while (lst)
	{
		if (lst->next == lst)
		{
			free(lst->buf);
			free(lst);
			lst = NULL;
		}
		else if (lst->next->fd == fd || fd == -1)
		{
			node = lst->next->next;
			free(lst->next->buf);
			free(lst->next);
			lst->next = node;
			if (fd != -1)
				break ;
		}
		else
			lst = lst->next;
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static t_list	*lst;
	char			*s;
	int				len;

	lst = list(lst, fd);
	if (!lst)
		return (free_list(lst, -1));
	if (!(lst->buf)[0] && read(fd, lst->buf, BUFFER_SIZE) <= 0)
		return (free_list(lst, fd));
	s = join(NULL, lst->buf);
	len = 1;
	while (!findchr(lst->buf, '\n') && len)
	{
		len = read(fd, lst->buf, BUFFER_SIZE);
		if (len == -1)
			return (free_list(lst, -1));
		(lst->buf)[len] = '\0';
		s = join(s, lst->buf);
	}
	if (len)
		s = substruntil(s, findchr(s, '\n'));
	strshift(lst->buf, findchr(lst->buf, '\n'));
	return (s);
}
