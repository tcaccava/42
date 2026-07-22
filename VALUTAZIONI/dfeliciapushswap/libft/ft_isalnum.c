/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:57:01 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/19 12:03:37 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*#include <stdio.h>*/
int	ft_isalnum(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
/*
int	main()
{
	printf ("%d\n", ft_isalnum('3'));
	printf ("%d\n", ft_isalnum('a'));
	printf ("%d\n", ft_isalnum('A'));
	printf ("%d\n", ft_isalnum(' '));
}*/
