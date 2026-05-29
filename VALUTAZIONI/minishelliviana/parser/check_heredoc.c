/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:14:31 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/23 15:20:37 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*generate_temp_filename(size_t *i)
{
	char	*str_i;
	char	*temp_file;

	(*i)++;
	str_i = ft_itoa((int)*i);
	temp_file = ft_strjoin("./temp_", str_i);
	free(str_i);
	return (temp_file);
}

static int	handle_heredoc_child(int fd, t_token **tokens, size_t *i,
		t_data *data)
{
	g_last_signal = 0;
	signal_manager(SIGINT, heredoc_handler);
	return (write_on_file(fd, (char *)tokens[*i]->content, tokens, data));
}

static void	update_token_content(t_token **tokens, size_t *i, char *temp_file)
{
	free(tokens[*i]->content);
	tokens[*i]->content = ft_strdup(temp_file);
	free(temp_file);
}

static	void	free_child(t_data *data, t_execute *info, char *file, int fd)
{
	close(fd);
	close(info->fd[0]);
	close(info->fd[1]);
	free(file);
	free(info->fd);
	free(info);
	free_array(data->env);
	free_tokens(data->tokens);
	free(data);
}

int	check_heredoc(t_token **tokens, size_t *i, t_data *data, t_execute *info)
{
	int		fd;
	char	*temp_file;
	int		ret;
	pid_t	pid;

	temp_file = generate_temp_filename(i);
	fd = open(temp_file, O_CREAT | O_RDWR, 0644);
	pid = fork();
	if (pid == 0)
	{
		ret = handle_heredoc_child(fd, tokens, i, data);
		free_child(data, info, temp_file, fd);
		exit(ret);
	}
	else
	{
		(*i)++;
		waitpid(pid, &ret, 0);
	}
	(*i)--;
	close(fd);
	update_token_content(tokens, i, temp_file);
	g_last_signal = 0;
	signal_manager(SIGINT, sigint_handler);
	return (ret);
}
