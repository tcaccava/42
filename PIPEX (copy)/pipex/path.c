/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:09:56 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/13 10:52:49 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_paths(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

char	*concat_path(char *dir, char *cmd)
{
	char	*path;
	int		len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = malloc(len);
	if (!path)
		return (NULL);
	ft_strcpy(path, dir);
	ft_strcat(path, "/");
	ft_strcat(path, cmd);
	return (path);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	paths = get_paths(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = concat_path(paths[i], cmd);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}
