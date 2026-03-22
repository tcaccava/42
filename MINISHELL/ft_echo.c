/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:09:32 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:11:46 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_n_options(char **args)
{
	int	i;

	i = 1;
	if (!args || !args[1])
		return (0);
	while (args[i] && is_valid_n_option(args[i]))
		i++;
	return (i - 1);
}

int	save_std_fds(int *stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	*stdout_copy = dup(STDOUT_FILENO);
	if (*stdin_copy == -1 || *stdout_copy == -1)
	{
		perror("minishell: dup");
		if (*stdin_copy != -1)
			close(*stdin_copy);
		if (*stdout_copy != -1)
			close(*stdout_copy);
		return (1);
	}
	return (0);
}

void	restore_std_fds(int stdin_copy, int stdout_copy)
{
	if (stdin_copy != -1)
	{
		dup2(stdin_copy, STDIN_FILENO);
		close(stdin_copy);
	}
	if (stdout_copy != -1)
	{
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdout_copy);
	}
}

static int	handle_echo_output(t_tree_node *root, int n_option)
{
	int	i;

	i = 1 + n_option;
	while (root->cmd->args[i])
	{
		printf("%s", root->cmd->args[i]);
		if (root->cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (n_option == 0)
		printf("\n");
	return (0);
}

int	ft_echo(t_tree_node *root)
{
	int	n_option;
	int	stdin_copy;
	int	stdout_copy;

	stdin_copy = -1;
	stdout_copy = -1;
	if (!root->cmd || !root->cmd->args)
		return (1);
	if (root->cmd->input_file || root->cmd->output_file
		|| root->cmd->heredoc_limiter)
	{
		if (save_std_fds(&stdin_copy, &stdout_copy) != 0)
			return (1);
		setup_redirections(root->cmd);
	}
	n_option = check_n_options(root->cmd->args);
	handle_echo_output(root, n_option);
	restore_std_fds(stdin_copy, stdout_copy);
	return (0);
}
