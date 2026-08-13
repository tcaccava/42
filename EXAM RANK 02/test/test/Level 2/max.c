/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   max.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:51:04 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/19 22:49:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

int		max(int* tab, unsigned int len)
{
    if (len == 0)
        return 0;
    int max;
    size_t i = 0;
    max = tab[0];
    while(i < len)
    {
        if (tab[i] > max)
            max = tab[i];
        i++;
    }
    return max;
}

int main()
{
    int test[] = {1,2,3,342,976};
    int test2[] = {};
    printf("%d\n", max(test2, 0));
    printf("%d\n", max(test, 5));
    return 0;
}