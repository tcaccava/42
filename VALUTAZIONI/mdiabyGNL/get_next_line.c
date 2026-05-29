/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:01:04 by mdiaby            #+#    #+#             */
/*   Updated: 2025/05/05 03:50:42 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*buff_join(char *collected_data, char *buffer)
{
	char	*res;

	if (!collected_data)
		return (ft_strdup(buffer));
	res = ft_strjoin(collected_data, buffer);
	if (!res)
	{
		free(collected_data);
		return (NULL);
	}
	free(collected_data);
	return (res);
}

static char	*fetch_line(char **collected_data)
{
	char			*line;
	char			*leftover_data;
	size_t			i;

	i = 0;
	while ((*collected_data)[i] != '\n' && (*collected_data)[i] != '\0')
		i++;
	line = malloc((i + 2) * sizeof(char));
	if (!line)
	{
		free(*collected_data);
		return (NULL);
	}
	ft_strlcpy(line, *collected_data, i + 2);
	if ((*collected_data)[i] == '\n')
		leftover_data = ft_strdup(&(*collected_data)[i + 1]);
	else
		leftover_data = NULL;
	free(*collected_data);
	*collected_data = leftover_data;
	return (line);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	static char	*collected_data;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes = read(fd, buffer, BUFFER_SIZE);
	while (bytes > 0)
	{
		buffer[bytes] = '\0';
		collected_data = buff_join(collected_data, buffer);
		if (ft_strchr(collected_data, '\n'))
		{
			free(buffer);
			return (fetch_line(&collected_data));
		}
		bytes = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (collected_data && *collected_data != '\0')
		return (fetch_line(&collected_data));
	return ((free(collected_data), collected_data = NULL));
}
int main()
{
	
}