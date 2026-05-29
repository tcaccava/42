/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 09:17:21 by lonulli           #+#    #+#             */
/*   Updated: 2024/12/23 09:17:22 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*Function to update stash to the next new line.
It also handle the case where the end of file is reached.
Paramaters: Takes stash and make a new one out of it.
Return value: New stash. */

char	*update_stash(char *stash)
{
	int		i;
	char	*new_stash;
	int		j;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] != '\n' && stash[i] != '\0')
		i++;
	if (stash[i] == '\n')
		i += 1;
	j = i;
	while (stash[j])
		j++;
	new_stash = malloc(sizeof(char) * (j - i) + 1);
	if (((j - i) == 0 && !ft_strchr(stash, '\n')))
	{
		free(new_stash);
		return (NULL);
	}
	j = 0;
	while (stash[i])
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	return (new_stash);
}

/*Function to calculate lenght of a string until a "\n" is met.
Takes a string as parameter. Return the start of the next line we've to print.
Done by adding +1 to the index where we find the new line.*/

int	linelen(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	return (i + 1);
}

/*Extract line we'll get the string out of the current stash.
Will also free the old_line
Parameters: String stash, from where we'll extract the line to print.
Old line, which is the old line we've to free.(The buffer where we've used read.
Return values: Line we've to print.*/

char	*extract_line(char *str, char *old_line)
{
	int		i;
	int		linelenght;
	char	*line;

	free(old_line);
	if (!str || ft_strlen(str) == 0)
		return (NULL);
	i = 0;
	if (str)
		linelenght = linelen(str);
	else
		linelenght = ft_strlen(str);
	line = malloc(sizeof(char) * linelenght + 1);
	while (str[i] != '\n' && str[i] != '\0')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
		line[i] = '\n';
	else
		line[i] = '\0';
	line[i + 1] = '\0';
	return (line);
}

/*Function that will join stash with buffer and also free previous buffer.
Parameters: Old stash, and the buffer to join with the string.
Return values: New stash.*/

char	*join_and_free(char *stash, char *buf)
{
	char	*temp;

	temp = stash;
	stash = ft_strjoin(stash, buf);
	free(temp);
	return (stash);
}

/* Workflow of main function:
1) Check for errors before making any read call.
2) Allocate enough space for the line we'll join to the stash.
3) Initial read call to enter loop.
4) Null terminate current buffer("line") and join it to stash.
5) When a new line is met we break, else we keep reading.
6) We've now a stash, from where to extract the line.
7) We updated the new stash by removing until "\n"
8) Free (temp) which is the old stash. <-- Only made to free last stash call
9) Almost all frees happens inside sub-functions.
10) Return the new line. */

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;
	int			bytes_read;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = safe_malloc();
	if (line)
		bytes_read = read(fd, line, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		line[bytes_read] = '\0';
		stash = join_and_free(stash, line);
		if (ft_strchr(line, '\n'))
			break ;
		bytes_read = read(fd, line, BUFFER_SIZE);
	}
	line = extract_line(stash, line);
	temp = stash;
	stash = update_stash(stash);
	free(temp);
	return (line);
}
