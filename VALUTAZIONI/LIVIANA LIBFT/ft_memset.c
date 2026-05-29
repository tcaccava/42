/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:11:27 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 17:13:52 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	insert;
	unsigned char	*pointer;
	size_t			index;

	insert = (unsigned char)c;
	pointer = (unsigned char *) s;
	index = 0;
	while (index < n)
	{
		pointer[index] = insert;
		index++;
	}
	return (s);
}

/*#include "string.h"

int main()
{
//	void	*str;
	//char	array[20] = "ciao a te";
	int		list[] = {1, 2, 3};
//	size_t	n;

//	str = array;
//	n = 5;
	ft_memset(list, 0, 5);
	//memset(list, 0, 5);
	printf("%lu\n", sizeof(int));
	printf("%d %d %d", list[0], list[1], list[2]);
}*/
