/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush04.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loguicha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 18:00:36 by loguicha          #+#    #+#             */
/*   Updated: 2026/10/11 18:00:59 by loguicha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_putchar(char c);

int	check_params(int x, int y)
{
	if (!x || !y || x < 0 || y < 0)
		return (0);
	return (1);
}

void	ft_write_row(int width, char left, char middle, char right)
{
	char	lr;

	lr = '\n';
	ft_putchar(left);
	while (width && width > 2)
	{
		ft_putchar(middle);
		width--;
	}
	if (width > 1)
		ft_putchar(right);
	ft_putchar(lr);
}

void	rush(int x, int y)
{
	int	height;

	if (!check_params(x, y))
	{
		ft_putchar('A');
		return ;
	}
	height = y;
	ft_write_row(x, 'A', 'B', 'C');
	while (y && y > 2)
	{
		ft_write_row(x, 'B', ' ', 'B');
		y--;
	}
	if (height > 1)
		ft_write_row(x, 'C', 'B', 'A');
}
