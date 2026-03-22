/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:49:56 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/26 19:58:21 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_status(char *result, int exit_status)
{
	char	*status_str;
	char	*temp;

	status_str = ft_itoa(exit_status);
	if (!status_str)
		return (free(result), NULL);
	temp = ft_strjoin(result, status_str);
	free(result);
	free(status_str);
	return (temp);
}

char	*expand_var_name(char *token_value, int *i)
{
	int		start;
	int		end;
	char	*var_name;

	start = *i + 1;
	end = start;
	while (token_value[end] && ((token_value[end] >= 65
				&& token_value[end] <= 90) || (token_value[end] >= 97
				&& token_value[end] <= 122) || (token_value[end] >= 48
				&& token_value[end] <= 57) || token_value[end] == '_'))
		end++;
	var_name = ft_strndup(&token_value[start], end - start);
	*i = end;
	return (var_name);
}

char	*expand_env_var(char *result, char *var_name, char **envp)
{
	char	*var_value;
	char	*temp;
	int		i;
	int		var_size;

	var_size = ft_strlen(var_name);
	i = 0;
	var_value = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_size) == 0
			&& envp[i][var_size] == '=')
		{
			var_value = envp[i] + var_size + 1;
			break ;
		}
		i++;
	}
	if (var_value)
		temp = ft_strjoin(result, var_value);
	else
		temp = ft_strjoin(result, "");
	free(result);
	return (temp);
}

char	*append_char(char *result, char c)
{
	char	*temp;

	temp = ft_strjoin_char(result, c);
	free(result);
	return (temp);
}

char	*expand_variables(char *token_value, char **envp, int exit_status)
{
	t_expand_args	args;
	char			*result;

	args.i = 0;
	args.exit_status = exit_status;
	args.envp = envp;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (token_value[args.i])
	{
		if (token_value[args.i] == '$')
			result = handle_dollar_sign(token_value, result, &args);
		else
		{
			result = append_char(result, token_value[args.i]);
			args.i++;
		}
		if (!result)
			return (NULL);
	}
	return (result);
}
