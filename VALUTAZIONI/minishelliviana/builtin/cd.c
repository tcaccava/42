/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:03:13 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/20 19:36:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_previous_directory(char *curr_dir)
{
	char	*end;
	char	*prev;
	int		len;

	len = ft_strlen(curr_dir);
	end = curr_dir + len;
	while (*end != '/')
	{
		end--;
		len--;
	}
	prev = ft_substr(curr_dir, 0, len);
	if (*prev == '\0')
	{
		free(prev);
		prev = malloc(2 *(sizeof(char)));
		*prev = '/';
		*(prev + 1) = '\0';
	}
	return (prev);
}

void	cd(char **str, t_data *data, t_execute *info)
{
	char	*new_dir;
	char	*curr_dir;
	char	*temp;

	curr_dir = malloc(1024 * (sizeof(char)));
	getcwd(curr_dir, 1024);
	temp = ft_getenv("HOME", data->env);
	if (!str || !*str || (**str == '~' && str[0][1] == '\0'))
		new_dir = ft_getenv("HOME", data->env);
	else if (str[1] != NULL)
	{
		write(2, "cd: too many arguments\n", 24);
		data->exit_status = 1;
		return (free(curr_dir), free(temp));
	}
	else if (**str == '~')
		new_dir = ft_strjoin(temp, *str + 1);
	else if (ft_strncmp(*str, "..", ft_strlen("..")) == 0)
		new_dir = get_previous_directory(curr_dir);
	else
		new_dir = ft_strdup(*str);
	check_error(chdir(new_dir), "cd: ", new_dir, data);
	update_pwd(new_dir, info, data);
	data->exit_status = 0;
	return (free(curr_dir), free(new_dir), free(temp));
}
