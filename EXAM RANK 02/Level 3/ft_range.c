/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:28:54 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 11:36:49 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int *ft_range(int start, int end)
{
    int *array;
    long long i = 0;
    long long len;
    
    // Calcola la lunghezza usando long long per evitare overflow
    if (start <= end)
        len = (long long)end - (long long)start + 1;
    else
        len = (long long)start - (long long)end + 1;
    
    // Alloca la memoria
    array = (int *)malloc(sizeof(int) * len);
    if (!array)
        return (NULL);
    
    // Riempi l'array usando long long per i calcoli
    while (i < len)
    {
        if (start <= end)
            array[i] = ((long long)start + i);
        else
            array[i] = ((long long)start - i);
        i++;
    }
    
    return (array);
}

int main()
{
    int *array = ft_range(INT_MIN, 0);
    if (!array)
    {
        printf("Allocazione fallita\n");
        return (1);
    }
    
    // Stampa i primi 10 numeri per verifica
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    free(array);
    return (0);
}