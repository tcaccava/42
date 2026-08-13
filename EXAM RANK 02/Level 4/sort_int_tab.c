/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_int_tab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:50:36 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 14:15:47 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

void sort_int_tab(int *tab, unsigned int size)
{
    int i = 0;
    int temp;
    while (i < size - 1)
    {
        if(tab[i] > tab[i + 1])
        {
            temp = tab[i];
            tab[i] = tab[i + 1];
            tab[i + 1] = temp;
            i = 0;
        }
        else
            i++;
    }
}
int main()
{
    int tab[] = {3, 2, 5, 5, 8, 1};
    int size = 6;
    sort_int_tab(tab, size);
    printf("Array dopo l'ordinamento:\n");
    for (unsigned int i = 0; i < size; i++)
        printf("%d ", tab[i]);
    printf("\n");
}