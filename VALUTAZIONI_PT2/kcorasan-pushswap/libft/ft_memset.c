/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:32:37 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/27 15:50:12 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>
#include <string.h>

void	*ft_memset(void *s, int c, size_t len)
{
	size_t			i;
	unsigned char	*tmp;

	i = 0;
	tmp = (unsigned char *)s;
	while (i < len)
	{
		tmp[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
// int main()
//{
//   char f[] = "cisas";
//   char *ptr;
//   ptr = ft_memset(f, 65, 5); //come output verranno fuori tutte le A 65 = A
//   printf("gwrgr %s\n", ptr);
//   return (0)}
