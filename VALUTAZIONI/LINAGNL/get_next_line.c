/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 21:33:38 by ligharbi          #+#    #+#             */
/*   Updated: 2025/02/01 05:06:51 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_extract_line(char *str)
{
	int		i;
	char	*line;

	if (str == NULL || str[0] == '\0')
		return (NULL);
	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
		i++;
	line = (char *) ft_calloc((i + 2), sizeof(char));
	if (line == NULL)
		return (NULL);
	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
		line[i] = '\n';
	return (line);
}

char	*ft_after(char *str)
{
	int		i;
	int		j;
	char	*after;

	i = 0;
	j = 0;
	if (str == NULL)
		return (NULL);
	while (str[i] != '\n' && str[i])
		i++;
	if (str[i] == '\0')
		return (free(str), NULL);
	after = (char *)ft_calloc(ft_strlen(str) - i + 1, sizeof(char));
	if (!after)
		return (NULL);
	i++;
	while (str[i])
		after[j++] = str[i++];
	return (free(str), after);
}

char	*ft_addbuffer(char *str, char *buf)
{
	char	*newstr;

	if (!str || !buf)
		return (NULL);
	newstr = ft_strjoin(str, buf);
	if (newstr == NULL)
		return (free(str), str = NULL, NULL);
	free (str);
	return (newstr);
}

char	*ft_parse(int fd, char *str, int *n)
{
	char	*buf;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(str), NULL);
	*n = 1;
	while (*n > 0)
	{
		*n = read(fd, buf, BUFFER_SIZE);
		if (*n == -1)
			return (free(str), free(buf), NULL);
		buf[*n] = '\0';
		str = ft_addbuffer(str, buf);
		if (str == NULL)
			return (free(buf), NULL);
		if (ft_strchr(str, '\n'))
			break ;
	}
	free (buf);
	return (str);
}

char	*get_next_line(int fd)
{
	char		*ret;
	static char	*str;
	int			n;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (free(str), str = NULL, NULL);
	n = 0;
	if (str == NULL)
	{
		str = ft_calloc(1, 1);
		if (str == NULL)
			return (NULL);
	}
	if (!ft_strchr(str, '\n'))
		str = ft_parse(fd, str, &n);
	if (str == NULL)
		return (NULL);
	ret = ft_extract_line(str);
	str = ft_after(str);
	if (ret == NULL && n == 0)
		return (free(str), str = NULL, NULL);
	return (ret);
}

/*char	*ft_extract_line(char *str)
{
	int		i;
	int		dim;
	char	*line;

	if (str == NULL || str[0] == '\0')
		return (NULL);
	dim = 0;
	while (str[dim] != '\0' && str[dim] != '\n')
		dim++;
	if(str[dim] == '\n')
		dim ++;
	line = (char *) ft_calloc((dim + 1), sizeof(char));
	if(line == NULL)
		return (NULL);
	i = 0;
	while (i <= dim)
	{
		line[i] = str[i];
		i++;
	}
	return(line);
}
*/

/*
char	*ft_after(char *str)
{
	int	i;
	int	dim;
	int j;
	char	*after;

	i = 0;
	dim = 0;
	j = 0;
	if (str == NULL)
		return (NULL);
	if (str[0] == '\0')
		return (free(str), NULL);
	while(str[i] != '\n' && str[i])
		i++;
	if(str[i] == '\0')
		return(free(str), NULL);
	i++;
	while(str[i + dim])
		dim++;
	if (dim == 0)
		return (free(str), str = NULL, NULL);
	after = (char *)ft_calloc((dim +1), sizeof(char));
	if(!after)
		return (free(str), NULL);
	while(i < dim)
		after[j++]=str[i++];
	return(free(str), after);
}
*/
