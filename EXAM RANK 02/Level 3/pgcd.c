/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pgcd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:46:03 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 17:31:04 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int c, char **v)
{
    int nbr1 = atoi(v[1]);
    int nbr2 = atoi(v[2]);
    if(c == 3)
    {
        if (nbr1 > 0 && nbr2 > 0)
        {
           while(nbr2 != 0)
           {
            int temp = nbr2;
            nbr2 = nbr1 % nbr2;
            nbr1 = temp;
           }
           printf("%d", nbr1);
        }
    }
    printf("\n");
    return 0;
}

