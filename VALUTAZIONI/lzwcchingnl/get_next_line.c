/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzecchin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:34:00 by lzecchin          #+#    #+#             */
/*   Updated: 2025/04/28 18:23:46 by lzecchin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*saved;
	char		*buffer;
	int			n;

	buffer = malloc(BUFFER_SIZE);
	if (!buffer)
		return (NULL);
	n = 1;
	while ((n != 0) && (!ft_strchr(saved, '\n')))
	{
		n = read(fd, buffer, BUFFER_SIZE);
		if ((n < 0) || (n == 0 && saved == 0)
			|| ((n == 0) && (saved != 0) && !(*saved)))
		{
			free(buffer);
			return (NULL);
		}
		if (!ft_concatenate(&saved, buffer, n))
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (ft_getline_update_saved(&saved, n));
}
