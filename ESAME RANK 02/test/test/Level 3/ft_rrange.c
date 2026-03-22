/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rrange.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:38:32 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 12:28:56 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

int     *ft_rrange(int start, int end)
{
    int *array;
    int size;
    if (start <= end)
        size = end - start + 1;
    else
        size = start - end + 1;
    array = malloc(sizeof(int) * size);
    if (!array)
        return (NULL);
    int i = 0;
    while (i < size)
    {
        if (start >= end)
            array[i] = i + end;
        else
            array[i] = end - i;
        i++;
    }
    return array;
}

int main()
{
    int *array = ft_rrange(0,0);
    int i = 0;
    int size = 1;
    while (i < size)
    {
        printf("%d", array[i]);
        i++;
    }
}