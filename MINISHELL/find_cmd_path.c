/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:09:23 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/27 03:56:15 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_direct_path(const char *cmd_path)
{
	if (strchr(cmd_path, '/'))
	{
		if (access(cmd_path, X_OK) == 0)
			return (strdup(cmd_path));
	}
	return (NULL);
}

static char	**get_path_dirs(char **env)
{
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i])
		return (ft_split(env[i] + 5, ':'));
	return (NULL);
}

static char	*try_path_dir(const char *dir, const char *cmd)
{
	char	*full_path;
	char	*path;

	full_path = ft_strjoin(dir, "/");
	if (!full_path)
		return (NULL);
	path = ft_strjoin(full_path, cmd);
	free(full_path);
	if (!path)
		return (NULL);
	if (access(path, X_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

static void	free_path_dirs(char **path_dirs)
{
	int	i;

	i = 0;
	while (path_dirs[i])
		free(path_dirs[i++]);
	free(path_dirs);
}

char	*find_cmd_path(t_tree_node *root, char **env)
{
	int		j;
	char	*path;
	char	**path_dirs;

	path = check_direct_path(root->cmd->args[0]);
	if (path)
		return (path);
	path_dirs = get_path_dirs(env);
	if (!path_dirs)
		return (NULL);
	j = 0;
	while (path_dirs[j])
	{
		path = try_path_dir(path_dirs[j], root->cmd->args[0]);
		if (path)
		{
			free_path_dirs(path_dirs);
			return (path);
		}
		j++;
	}
	free_path_dirs(path_dirs);
	return (NULL);
}
