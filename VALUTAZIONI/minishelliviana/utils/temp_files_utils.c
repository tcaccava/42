/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_files_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:44:32 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/23 16:05:05 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unlink_files(t_data *data)
{
	t_token	**tokens;
	size_t	i;

	tokens = data->tokens;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->sub_type == LIMITER)
			unlink((char *)tokens[i]->content);
		i++;
	}
}
