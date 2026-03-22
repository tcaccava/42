/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:38:34 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/26 19:37:35 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_cmd(void)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->next = NULL;
	new_cmd->input_file = NULL;
	new_cmd->output_file = NULL;
	new_cmd->heredoc_limiter = NULL;
	new_cmd->append = 0;
	new_cmd->is_subshell = 0;
	new_cmd->next_separator = SEP_NONE;
	new_cmd->subshell_cmds = NULL;
	new_cmd->subshell_node = NULL;
	return (new_cmd);
}

void	add_cmd_to_list(t_command **list, t_command *new_cmd)
{
	t_command	*tmp;

	if (!*list)
	{
		*list = new_cmd;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
}

void	add_to_args(t_command *cmd, char *arg)
{
	int		i;
	char	**new;

	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		return ;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		new[i] = cmd->args[i];
		i++;
	}
	new[i++] = ft_strdup(arg);
	new[i] = NULL;
	free(cmd->args);
	cmd->args = new;
}

char	*handle_dollar_sign(char *token_value, char *result,
		t_expand_args *args)
{
	char	*var_name;

	if (token_value[args->i + 1] == '?')
	{
		result = expand_status(result, args->exit_status);
		args->i = args->i + 2;
	}
	else if ((token_value[args->i + 1] >= 65 && token_value[args->i + 1] <= 90)
		|| (token_value[args->i + 1] >= 97 && token_value[args->i + 1] <= 122)
		|| token_value[args->i + 1] == '_')
	{
		var_name = expand_var_name(token_value, &(args->i));
		if (!var_name)
			return (free(result), NULL);
		result = expand_env_var(result, var_name, args->envp);
		free(var_name);
	}
	else
	{
		result = append_char(result, token_value[args->i]);
		args->i++;
	}
	return (result);
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;
	int	sign;

	res = 0;
	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == ' '))
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}
