/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:34:02 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/05/23 15:34:32 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	mouse_hook(int keycode, int x, int y, t_data *data)
{
	int			code;
	static char	dir;

	code = 0;
	(void)y;
	if ((x < WIDTH / 2 && keycode == 1) || keycode == 4)
		code = rotate_player(65361, data, dir);
	else if (keycode == 1 || keycode == 5)
		code = rotate_player(65363, data, dir);
	if (code)
		draw(data);
	return (0);
}
