/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:46 by faiello           #+#    #+#             */
/*   Updated: 2025/10/17 16:04:24 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdio.h>
#include <stdlib.h>

void	error_exit(char *message, t_game *game)
{
	if (message)
		write(2, message, ft_strlen(message));
	if (game)
		destroy_game(game);
	exit(EXIT_FAILURE);
}
