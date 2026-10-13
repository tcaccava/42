/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loguicha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 10:22:23 by loguicha          #+#    #+#             */
/*   Updated: 2026/10/11 17:51:47 by loguicha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	rush(int x, int y);

int	main(int argc, char *argv[])
{
	if (argc <= 1 || argc > 3 || !argv[1] || !argv[2])
		return (0);
	rush(atoi(argv[1]), atoi(argv[2]));
	return (0);
}
