/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:09:29 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:09:30 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_current_dir(char *buffer)
{
	if (getcwd(buffer, PATH_MAX) == NULL)
	{
		perror("minishell: getcwd");
		return (1);
	}
	return (0);
}

static int	cd_to_home(char **envp, const char *old_pwd)
{
	char	new_pwd[PATH_MAX];
	char	*home;

	home = my_getenv(envp, "HOME");
	if (home == NULL)
	{
		write(2, "minishell: cd: HOME not set\n", 28);
		return (1);
	}
	else if (chdir(home) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (get_current_dir(new_pwd))
		return (1);
	set_env_var(envp, "PWD", new_pwd);
	set_env_var(envp, "OLDPWD", old_pwd);
	return (0);
}

static int	cd_to_previous(char **envp, const char *old_pwd)
{
	char	new_pwd[PATH_MAX];
	char	*oldpwd_ptr;

	oldpwd_ptr = my_getenv(envp, "OLDPWD");
	if (oldpwd_ptr == NULL)
	{
		write(2, "minishell: cd: OLDPWD not set\n", 30);
		return (1);
	}
	if (chdir(oldpwd_ptr) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (get_current_dir(new_pwd))
		return (1);
	printf("%s\n", oldpwd_ptr);
	set_env_var(envp, "PWD", new_pwd);
	set_env_var(envp, "OLDPWD", old_pwd);
	return (0);
}

static int	cd_to_path(char **envp, const char *path, const char *old_pwd)
{
	char	new_pwd[PATH_MAX];

	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (get_current_dir(new_pwd))
		return (1);
	set_env_var(envp, "PWD", new_pwd);
	set_env_var(envp, "OLDPWD", old_pwd);
	return (0);
}

int	ft_cd(t_tree_node *root, char **envp)
{
	char	old_pwd[PATH_MAX];

	if (get_current_dir(old_pwd))
		return (1);
	if (root->cmd->args[1] == NULL)
		return (cd_to_home(envp, old_pwd));
	else if (root->cmd->args[1][0] == '-' && root->cmd->args[1][1] == '\0')
		return (cd_to_previous(envp, old_pwd));
	else
		return (cd_to_path(envp, root->cmd->args[1], old_pwd));
}
