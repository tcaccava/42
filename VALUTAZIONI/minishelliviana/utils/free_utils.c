/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:41:51 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/19 15:41:55 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_data(t_data *data)
{
	if (variables)
		free_array(data->variables);
	if (variables)
		free_array(data->env);
	if (tokens)
		free_tokens(data->tokens);
	return (0);
}
