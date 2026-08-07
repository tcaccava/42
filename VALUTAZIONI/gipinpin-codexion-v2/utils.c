/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:34 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:34 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

size_t	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	putstr_fd(int fd, char *s)
{
	int	len;

	if (!s)
		return ;
	len = ft_strlen(s);
	write(fd, s, len);
}

void	*ft_calloc(size_t n_mem, size_t size)
{
	void	*ptr;
	size_t	tot;

	if (!(n_mem && size))
		return (NULL);
	if (size > (size_t)-1 / n_mem)
		return (NULL);
	tot = n_mem * size;
	ptr = malloc(tot);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, tot);
	return (ptr);
}

void	putnbr_fd(int fd, long n)
{
	char			c;

	if (n < 0)
		return ;
	if (n >= 10)
		putnbr_fd(fd, n / 10);
	c = n % 10 + '0';
	write(fd, &c, 1);
}

long	atol_check(char *s)
{
	int		i;
	long	res;
	int		has_digits;	

	i = 0;
	res = 0;
	has_digits = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
		i++;
	if (s[i] == '-')
		return (-1);
	else if (s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		has_digits = 1;
		if (res > (LONG_MAX - (s[i] - '0')) / 10)
			return (-1);
		res = res * 10 + (s[i] - '0');
		i++;
	}
	if (s[i] != '\0' || !has_digits)
		return (-1);
	return (res);
}
