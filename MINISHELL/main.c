/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:32:04 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/27 10:03:08 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_tree(t_tree_node *root, char **env)
{
	int	status;

	status = 0;
	if (!root)
		return (0);
	if (root->type == NODE_COMMAND)
		status = execute_command_node(root, env);
	else if (root->type == NODE_PIPE)
		status = execute_pipe(root, env);
	else if (root->type == NODE_AND)
	{
		status = execute_tree(root->left, env);
		if (status == 0)
			status = execute_tree(root->right, env);
	}
	else if (root->type == NODE_OR)
	{
		status = execute_tree(root->left, env);
		if (status != 0)
			status = execute_tree(root->right, env);
	}
	return (status);
}

static void	cleanup_resources(char *line, t_token *tokens, t_tree_node *root,
		t_command *cmds)
{
	(void)cmds;
	if (line)
		free(line);
	if (tokens)
		free_tokens(tokens);
	if (root)
		free_tree(root);
}

static void	exec_tokens(t_token *tokens, char **envp, int *status)
{
	t_command	*cmds;
	t_tree_node	*root;

	cmds = NULL;
	root = NULL;
	if (tokens)
	{
		cmds = parse(tokens, envp, *status);
		if (cmds)
		{
			root = build_tree(cmds);
			if (root)
				*status = execute_tree(root, envp);
		}
	}
	cleanup_resources(NULL, tokens, root, cmds);
}

int	process_command_line(char **envp, int *status)
{
	char	*line;
	t_token	*tokens;

	handle_signals();
	line = readline("minishell> ");
	if (!line)
	{
		printf("exit\n");
		return (0);
	}
	if (*line)
		add_history(line);
	tokens = tokenize(line);
	if (!tokens)
		return (0);
	exec_tokens(tokens, envp, status);
	free(line);
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	int		status;
	char	**envp;

	(void)argc;
	(void)argv;
	envp = copy_env(env);
	if (!envp)
		return (1);
	status = 0;
	setup_signals();
	while (1)
	{
		if (!process_command_line(envp, &status))
			break ;
	}
	rl_clear_history();
	free_env(envp);
	return (status);
}
