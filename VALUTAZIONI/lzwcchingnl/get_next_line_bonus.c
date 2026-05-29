/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzecchin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:30:07 by lzecchin          #+#    #+#             */
/*   Updated: 2025/04/28 18:30:09 by lzecchin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	*saved[MAX_FD];
	char		*buffer;
	int			n;

	buffer = malloc(BUFFER_SIZE);
	if (!buffer)
		return (NULL);
	n = 1;
	while ((n != 0) && (!ft_strchr(saved[fd], '\n')))
	{
		n = read(fd, buffer, BUFFER_SIZE);
		if ((n < 0) || (n == 0 && saved[fd] == 0)
			|| ((n == 0) && (saved[fd] != 0) && !(*saved[fd])))
		{
			free(buffer);
			return (NULL);
		}
		if (!ft_concatenate(&(saved[fd]), buffer, n))
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (ft_getline_update_saved(&(saved[fd]), n));
}
