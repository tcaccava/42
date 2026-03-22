/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_power_of_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 20:28:45 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 02:36:08 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
int is_power_of_2(unsigned int n)
{
    if (n == 0)
        return 0;
    while( n % 2 == 0)
        n = n / 2;
    if (n == 1)
    {
        return 1;
    }
    else
        return 0;
}
int main()
{
    printf("%i\n", is_power_of_2(8));
    printf("%i\n", is_power_of_2(9));
}
/*
versione alternativa fatta da me 
int ispower2(int n)
{
    if (n < 2)
        return 0;
    while (n % 2 == 0)
    {
        n = n / 2;
        if (n == 1)
            return 1; 
    }
    return 0 ;   
}
*/