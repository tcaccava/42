/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:08:08 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 14:51:40 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

unsigned int    lcm(unsigned int a, unsigned int b)
{
    unsigned int lcm;
    if(a == 0 || b == 0)
        lcm = 0;
    if (a > b)
        lcm = a;
    else
        lcm = b;
    while (1)
    {
        if (lcm % a == 0 && lcm % b == 0)
            return lcm;
        ++lcm;
    }
}
int main()
{
    printf("%u", lcm(12, 18));
    return 0;    
}