/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:33:50 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:53:43 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipe_left(int *pipefd, t_tree_node *left, char **env)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	if (left->type == NODE_COMMAND && (left->cmd->input_file
			|| left->cmd->heredoc_limiter || left->cmd->output_file))
		setup_redirections(left->cmd);
	exit(execute_tree(left, env));
}

void	execute_pipe_right(int *pipefd, t_tree_node *right, char **env)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	if (right->type == NODE_COMMAND && (right->cmd->input_file
			|| right->cmd->heredoc_limiter || right->cmd->output_file))
		setup_redirections(right->cmd);
	exit(execute_tree(right, env));
}

int	setup_pipe_processes(int pipefd[2], t_tree_node *root, char **env)
{
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork();
	if (pid1 == -1)
	{
		perror("Error fork left");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	else if (pid1 == 0)
		execute_pipe_left(pipefd, root->left, env);
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("Error fork right");
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(pid1, NULL, 0);
		return (-1);
	}
	else if (pid2 == 0)
		execute_pipe_right(pipefd, root->right, env);
	return (pid2);
}

int	execute_pipe(t_tree_node *root, char **env)
{
	int		pipefd[2];
	int		status;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		return (1);
	}
	pid2 = setup_pipe_processes(pipefd, root, env);
	if (pid2 == -1)
		return (1);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid2, &status, 0);
	return (status);
}
