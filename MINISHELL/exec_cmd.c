/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:22:32 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 20:39:26 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_subshell(t_tree_node *root, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
	{
		if (root->cmd->input_file || root->cmd->output_file
			|| root->cmd->heredoc_limiter)
			setup_redirections(root->cmd);
		exit(execute_tree(root->cmd->subshell_node, env));
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_builtin(t_tree_node *root, char **env)
{
	if (strcmp(root->cmd->args[0], "echo") == 0)
		return (ft_echo(root));
	else if (strcmp(root->cmd->args[0], "cd") == 0)
		return (ft_cd(root, env));
	else if (strcmp(root->cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (strcmp(root->cmd->args[0], "export") == 0)
		return (ft_export(root, env));
	else if (strcmp(root->cmd->args[0], "unset") == 0)
		return (ft_unset(root, env));
	else if (strcmp(root->cmd->args[0], "env") == 0)
		return (ft_env(env));
	else if (strcmp(root->cmd->args[0], "exit") == 0)
		return (ft_exit(root, env));
	return (-1);
}

int	execute_command(t_tree_node *root, char **env)
{
	int	builtin_result;

	if (root->cmd->is_subshell)
		return (execute_subshell(root, env));
	if (!root->cmd || !root->cmd->args || !root->cmd->args[0])
		return (1);
	builtin_result = execute_builtin(root, env);
	if (builtin_result != -1)
		return (builtin_result);
	return (execute_external_command(root, env));
}
