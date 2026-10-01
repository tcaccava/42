/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:07:26 by mpapale           #+#    #+#             */
/*   Updated: 2026/09/27 16:01:28 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*obtain_remaining(char *magazzino_e_extra)
{
	int		i;
	char	*resto;

	if (!magazzino_e_extra)
		return (NULL);
	if (!*magazzino_e_extra)
	{
		free(magazzino_e_extra);
		return (NULL);
	}
	i = 0;
	while (magazzino_e_extra[i] && magazzino_e_extra[i] != '\n')
		i++;
	if (!magazzino_e_extra[i] || !magazzino_e_extra[i + 1])
	{
		free(magazzino_e_extra);
		return (NULL);
	}
	resto = ft_strdup(&magazzino_e_extra[i + 1]);
	free(magazzino_e_extra);
	return (resto);
}

char	*extract_line(char *magazzino_e_extra)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	if (!magazzino_e_extra || !*magazzino_e_extra)
		return (NULL);
	while (magazzino_e_extra[i] && magazzino_e_extra[i] != '\n')
		i++;
	if (magazzino_e_extra[i] == '\n')
		i++;
	tmp = malloc(sizeof(char) * (i + 1));
	if (!tmp)
		return (NULL);
	j = 0;
	while (j < i)
	{
		tmp[j] = magazzino_e_extra[j];
		j++;
	}
	tmp[j] = '\0';
	return (tmp);
}

char	*read_and_store(int fd, char *magazzino_e_extra)
{
	char	*buffer;
	int		byte_letti;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	byte_letti = 1;
	while (!ft_strchr(magazzino_e_extra, '\n') && byte_letti > 0)
	{
		byte_letti = read(fd, buffer, BUFFER_SIZE);
		if (byte_letti == -1)
		{
			free(buffer);
			free(magazzino_e_extra);
			magazzino_e_extra = NULL;
			return (NULL);
		}
		if (byte_letti == 0)
			break ;
		buffer[byte_letti] = '\0';
		magazzino_e_extra = ft_strjoin_gnl(magazzino_e_extra, buffer);
	}
	free(buffer);
	return (magazzino_e_extra);
}

char	*get_next_line(int fd)
{
	static char	*magazzino_e_extra;
	char		*riga_da_restituire;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	magazzino_e_extra = read_and_store(fd, magazzino_e_extra);
	if (!magazzino_e_extra)
		return (NULL);
	riga_da_restituire = extract_line(magazzino_e_extra);
	magazzino_e_extra = obtain_remaining(magazzino_e_extra);
	return (riga_da_restituire);
}
