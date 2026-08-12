/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 16:01:48 by grocca            #+#    #+#             */
/*   Updated: 2026/08/11 13:48:03 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	n_len(int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
		len++;
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

int	ft_putnbr_fd(int n, int fd)
{
	char	p[12];
	size_t	len;

	if (n == -2147483648)
		return (write(fd, "-2147483648", 11));
	else if (n == 0)
		return (write(fd, "0", 1));
	else
	{
		len = n_len(n);
		p[len] = '\0';
		if (n < 0)
		{
			p[0] = '-';
			n = -n;
		}
		while (n)
		{
			p[len - 1] = '0' + n % 10;
			n = n / 10;
			len--;
		}
		return (write(fd, p, ft_strlen(p)));
	}
}

int	ft_function(int fd, const char *format, va_list ap)
{
	float	num;
	int		ret;

	format++;
	ret = -1;
	if (*format == '%')
		ret = write(2, "%", 1);
	else if (*format == 'd' || *format == 'i')
		ret = ft_putnbr_fd(va_arg(ap, int), fd);
	else if (*format == 'f')
	{
		num = va_arg(ap, double);
		if (ft_putnbr_fd(num, fd) == -1)
			return (-1);
		write(fd, ".", 1);
		num = num - (int)num;
		ret = (int)(num * 100);
		if (ret >= 0 && ret <= 9 && write(fd, "0", 1) == -1)
			return (-1);
		ret = ft_putnbr_fd((int)(num * 100), fd);
	}
	return (ret);
}

int	ft_printf(int fd, const char *format, ...)
{
	int		ret;
	int		count;
	va_list	ap;

	va_start(ap, format);
	count = 0;
	while (*format)
	{
		if (*format != '%')
			ret = write(fd, format, 1);
		else
		{
			ret = ft_function(fd, format, ap);
			format++;
		}
		if (ret == -1)
		{
			va_end(ap);
			return (-1);
		}
		count += ret;
		format++;
	}
	va_end(ap);
	return (count);
}
