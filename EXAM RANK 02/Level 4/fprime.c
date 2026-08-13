/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fprime.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 01:03:54 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 04:06:00 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void fprime(int n)
{
    if (n == 1)
        printf("1");
    int i = 2;
    while (n >= i)
    {
        if (n % i == 0)
        {
            printf("%d", i);
            n = n / i;
            if (n > 1)
                printf("*");
        }
        else 
            i++;
    }
}
int main(int c, char **v)
{
    if (c == 2)
    {
        int n = atoi(v[1]);
        fprime(n);
    }
    printf("\n");
    return (0);
}

int main(int c, char **v)
{
    if (c == 2)
    {
        int n = atoi(v[1]);
        int f = 2;
        if(n == 1)
            printf("1");
        while (n >= f)
        {
            if (n % f == 0)
            {
                n = n / f;
                printf("%i", f);
                if (n > 1)
                    printf("*");          
            }
            else
                f++;
        }
    }
    printf("\n");
    return 0;
}