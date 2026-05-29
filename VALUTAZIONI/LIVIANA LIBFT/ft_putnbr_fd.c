/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:29:13 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/16 13:05:20 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	printnumber(int c, int fd)
{
	char	n;

	n = (c % 10) + '0';
	if (c > 9)
		ft_putnbr_fd(c / 10, fd);
	ft_putchar_fd(n, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	printnumber(n, fd);
}

/*int main()
{
	ft_putnbr_fd(-42, 1);
}*/
