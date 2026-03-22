/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_extern_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:30:36 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/27 04:04:40 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_command_not_found(t_tree_node *root)
{
	write(2, "minishell: command not found: ", 30);
	write(2, root->cmd->args[0], strlen(root->cmd->args[0]));
	write(2, "\n", 1);
	return (127);
}

int	handle_child_process(t_tree_node *root, const char *path, char **env)
{
	set_default_signal_handlers();
	if (root->cmd->input_file || root->cmd->output_file
		|| root->cmd->heredoc_limiter)
		setup_redirections(root->cmd);
	if (execve(path, root->cmd->args, env) == -1)
	{
		perror("minishell");
		exit(1);
	}
	return (0);
}

int	handle_parent_process(pid_t pid, struct sigaction *sa_int,
		struct sigaction *sa_quit)
{
	int	status;

	ignore_signals();
	waitpid(pid, &status, 0);
	sigaction(SIGINT, sa_int, NULL);
	sigaction(SIGQUIT, sa_quit, NULL);
	return (handle_child_termination_signals(status));
}

int	execute_external_command(t_tree_node *root, char **env)
{
	pid_t				pid;
	char				*path;
	struct sigaction	sa_quit;
	struct sigaction	sa_int;

	path = find_cmd_path(root, env);
	if (!path)
		return (handle_command_not_found(root));
	save_orig_sig_handlers(&sa_int, &sa_quit);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free(path);
		return (1);
	}
	else if (pid == 0)
		return (handle_child_process(root, path, env));
	else
	{
		free(path);
		return (handle_parent_process(pid, &sa_int, &sa_quit));
	}
}
