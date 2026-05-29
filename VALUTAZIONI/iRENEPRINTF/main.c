/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:08:35 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/24 17:09:00 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"


int main()
{
	ft_printf("ft_printf: %s\n", "Hello world!");
	printf("printf: %s\n", "Hello world!");
	ft_printf("ft_printf: %d\n", 42);
	printf("printf: %d\n", 42);
	ft_printf("ft_printf: %c\n", 'A');
	printf("printf: %c\n", 'A');
	ft_printf("ft_printf: %u\n", 123456);
	printf("printf: %u\n", 123456);
	ft_printf("ft_printf: %x\n", 255);
	printf("printf: %x\n", 255);
	ft_printf("ft_printf %X\n", 255);
	printf("printf %X\n", 255);
	int a = 42;
	ft_printf("ft_printf: %p\n", (void *)&a);
	printf("printf: %p\n", (void *)&a);
	ft_printf("ft_printf: %%\n");
	printf("printf: %%\n");
	return 0;
}