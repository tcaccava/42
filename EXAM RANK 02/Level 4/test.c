/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 02:08:30 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 14:41:46 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
void sort_int_tab(int *tab, unsigned int size)
{
    int i = 0;
    int temp;
    while (i < size - 1)
    {
        if (tab[i] > tab[i + 1])
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
    int tab[] = {3, 2, 5, 7, 4};
    int size = 5;
    sort_int_tab(tab, size);
    int i = 0;
    while (i < size)
    {
        printf("%d", tab[i]);
        i++;
    }
}