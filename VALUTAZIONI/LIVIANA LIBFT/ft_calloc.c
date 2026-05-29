/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:22:25 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/16 20:16:38 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*pointer;

	if (size != 0 && nmemb > ((size_t) - 1) / size)
		return (NULL);
	pointer = malloc (nmemb * size);
	if (pointer == NULL)
		return (NULL);
	ft_bzero(pointer, nmemb * size);
	return (pointer);
}

/*int main()
{
	char *array;
	int n = 5;

	array = (char *) calloc (5, sizeof(char));
	while (n > 0)
	{
		printf("%c\n", *array);
		n--;
	}
}*/
