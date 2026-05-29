/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:35:24 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 17:37:09 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	int	i;

	i = (int)n;
	if (dest == NULL || src == NULL)
		return (NULL);
	if (src > dest)
		ft_memcpy(dest, src, n);
	if (src < dest)
	{
		src += i - 1;
		dest += i - 1;
		while (i > 0)
		{
			*(char *)dest = *(char *)src;
			dest--;
			src--;
			i--;
		}
		dest++;
	}
	return (dest);
}

/*int	main()
{
	char	dest[] = "a shiny white sphere";
	char	src[] = "ehi";

	ft_memmove(dest, src, 2);
	printf("%s", dest);0

#include <stdio.h>
int main(void)
{


	char s[] = {65, 66, 67, 68, 69, 0, 45};
	char s0[] = { 0,  0,  0,  0,  0,  0, 0};
	char sCpy[] = {65, 66, 67, 68, 69, 0, 45};
	char sResult[] = {67, 68, 67, 68, 69, 0, 45};
	char sResult2[] = {67, 67, 68, 68, 69, 0, 45};

	printf("%s\n",(char *)ft_memmove(sResult + 1, sResult, 2))
	write(1, "\n", 1);

	return (0);
}*/
