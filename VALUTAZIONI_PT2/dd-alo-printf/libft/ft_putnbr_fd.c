/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:15:32 by dd-alo            #+#    #+#             */
/*   Updated: 2026/07/28 19:03:35 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	recurs(long num, int fd)
{
	char	c;

	if (num > 9)
	{
		recurs(num / 10, fd);
		num %= 10;
	}
	c = num + '0';
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	num;

	num = n;
	if (num < 0)
	{
		write(fd, "-", 1);
		num *= -1;
	}
	if (num == 0)
		write(fd, "0", 1);
	else
		recurs(num, fd);
}
