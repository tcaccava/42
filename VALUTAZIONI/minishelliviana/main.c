/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:20:04 by marvin            #+#    #+#             */
/*   Updated: 2025/04/24 15:53:53 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_signal;

static void	start_main(t_data **data, t_execute **info)
{
	*data = malloc(sizeof(t_data));
	*info = malloc(sizeof(t_execute));
	initial_configuration(*data, *info);
}

static char	*ft_get_line(char *buf, t_data *data)
{
	char	*line;

	data->tokens = NULL;
	signal_manager(SIGINT, sigint_handler);
	signal_manager(SIGQUIT, SIG_IGN);
	line = readline(buf);
	return (line);
}

static int	tokens_handler(char *line, t_data *data, t_execute *info)
{
	if (!line)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (2);
	}
	if (line && *line && ft_strcmp(line, "\n"))
		add_history(line);
	if (tokenizer(line, &data->tokens))
		return (1);
	if (syntax_error(data->tokens, check_args(data->tokens)))
	{
		data->exit_status = 2;
		return (1);
	}
	reorder_tokens(data->tokens);
	assign_index(data->tokens);
	if (finalize_tokens(data->tokens, data, info) == 256)
	{
		unlink_files(data);
		free_tokens(data->tokens);
		return (1);
	}
	assign_args(data->tokens);
	return (0);
}

static void	execution_cicle(t_data *data, t_execute *info)
{
	size_t	i;

	i = 0;
	while (data->tokens[i])
	{
		signal_manager(SIGQUIT, sigquit_handler);
		g_last_signal = 0;
		data->tree = build_tree(data->tokens, &i);
		start_execution(info, data);
		data->tree = NULL;
		while (data->tokens[i] && (data->tokens[i]->type & (NEW_LINE | END)))
			i++;
	}
}

int	main(void)
{
	t_data		*data;
	char		*line;
	t_execute	*info;
	char		*buf;

	start_main(&data, &info);
	while (42)
	{
		buf = set_prompt();
		line = ft_get_line(buf, data);
		if (g_last_signal == SIGINT)
			data->exit_status = 130;
		else if (g_last_signal == SIGQUIT)
			data->exit_status = 131;
		free(buf);
		data->tokens_result = tokens_handler(line, data, info);
		if (data->tokens_result == 1)
			continue ;
		if (data->tokens_result == 2)
			break ;
		execution_cicle(data, info);
		unlink_files(data);
		free_tokens(data->tokens);
	}
	final_free(info, data);
}
