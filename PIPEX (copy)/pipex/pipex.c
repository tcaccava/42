/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 03:34:10 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/13 05:37:41 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_open(char *file, int flags, int mode)
{
	int	fd;

	(void)mode;
	fd = open(file, flags);
	if (fd == -1)
	{
		perror(file);
		exit(1);
	}
	return (fd);
}

void	ft_exec(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	path = find_path(args[0], envp);
	if (execve(path, args, envp) == -1)
	{
		perror("execve");
		exit(1);
	}
}
