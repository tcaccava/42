/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 05:39:13 by edveneto          #+#    #+#             */
/*   Updated: 2026/07/08 05:56:52 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_reader(int fd, char *stash)
{
	char	*buf;
	int		rd;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	rd = 1;
	while (buf && rd > 0 && !ft_charcheck(stash, '\n'))
	{
		rd = read(fd, buf, BUFFER_SIZE);
		if (rd == -1)
			break ;
		buf[rd] = '\0';
		if (rd > 0)
			stash = ft_strjoin(stash, buf);
		if (!stash)
			break ;
	}
	if (buf)
		free(buf);
	if (rd == -1 || !buf || !stash)
	{
		free(stash);
		return (NULL);
	}
	return (stash);
}

char	*ft_extractor(char *stash)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	if (!stash || !stash[i])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	j = -1;
	while (++j < i)
		line[j] = stash[j];
	line[j] = '\0';
	return (line);
}

char	*ft_cleaner(char *stash)
{
	int		i;
	int		j;
	char	*ns;

	i = 0;
	j = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i] || !stash[i + 1])
	{
		free(stash);
		return (NULL);
	}
	ns = malloc(sizeof(char) * (ft_strlen(stash) - i));
	if (ns)
	{
		while (stash[++i])
			ns[j++] = stash[i];
		ns[j] = '\0';
	}
	free(stash);
	return (ns);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*line;

	if (fd < 0 || fd > 1023 || BUFFER_SIZE <= 0)
		return (NULL);
	stash[fd] = ft_reader(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = ft_extractor(stash[fd]);
	if (!line)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	stash[fd] = ft_cleaner(stash[fd]);
	return (line);
}
