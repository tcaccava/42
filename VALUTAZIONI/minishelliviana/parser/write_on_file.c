/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_on_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:58:16 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/24 15:08:33 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_quotes_in_delimiter(char *delimiter)
{
	return (ft_strchr(delimiter, '"') || ft_strchr(delimiter, '\''));
}

static void	print_warning(char *delimiter)
{
	write(STDERR_FILENO, "minishell: warning: here-document", 33);
	write(STDERR_FILENO, " delimited by end-of-file ", 25);
	write(STDERR_FILENO, "(wanted `", 9);
	write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
	write(STDERR_FILENO, "')\n", 3);
}

static char	*get_line(char *delimiter)
{
	char	*line;

	line = readline(">");
	if (!line)
	{
		print_warning(delimiter);
		return (NULL);
	}
	return (line);
}

static void	process_line(int fd, char *line, int flag, t_data *data)
{
	if (flag == 0)
		line = check_export2(line, data);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

int	write_on_file(int fd, char *delimiter, t_token **tokens, t_data *data)
{
	char	*line;
	int		ret;
	int		flag;

	(void)tokens;
	ret = 0;
	flag = check_quotes_in_delimiter(delimiter);
	remove_quotes(delimiter);
	while (1)
	{
		line = get_line(delimiter);
		if (!line)
			return (0);
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		process_line(fd, line, flag, data);
	}
	return (ret);
}
