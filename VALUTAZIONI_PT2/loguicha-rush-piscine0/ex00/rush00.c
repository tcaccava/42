/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush00.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loguicha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 17:56:45 by loguicha          #+#    #+#             */
/*   Updated: 2026/10/11 17:57:56 by loguicha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_putchar(char c);

int	check_params(int x, int y)
{
	if (x <= 0 || y <= 0)
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
		ft_putchar('o');
		return ;
	}
	height = y;
	ft_write_row(x, 'o', '-', 'o');
	while (y && y > 2)
	{
		ft_write_row(x, '|', ' ', '|');
		y--;
	}
	if (height > 1)
		ft_write_row(x, 'o', '-', 'o');
}
