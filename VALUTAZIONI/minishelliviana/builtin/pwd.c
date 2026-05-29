/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:33:51 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/19 21:43:12 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_data *data, t_execute *info)
{
	char	*buf;

	buf = NULL;
	set_fd(info);
	buf = getcwd(buf, 1024);
	if (!buf)
		return ;
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	restore_fd(info);
	free(buf);
	data->exit_status = 0;
}

void	update_pwd(char *new_dir, t_execute *info, t_data *data)
{
	char	*temp;
	char	**array;

	temp = ft_strjoin("PWD=", new_dir);
	array = ft_split(temp, ' ');
	ft_export(array, data, info);
	free(temp);
	free_array(array);
}
