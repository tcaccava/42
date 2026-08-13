/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 02:36:57 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 03:58:53 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

int length(int n)
{
    int len = 0;
    if (n < 0)
        len++;
    if (n == 0)
        return 1;
    while (n != 0)
    {
        n = n / 10;
        len++;
    }
    return len;
}

char *ft_itoa(int nbr)
{
    int len = length(nbr);
    char *str = malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    if (nbr == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    if (nbr < 0)
    {
        str[0] = '-';
        nbr = -nbr;
    }
    int i = len - 1;
    str[len] = '\0';
    while (nbr != 0)
    {
        str[i] = nbr % 10 + '0';
        nbr = nbr / 10;
        i--;   
    }
    return str;
}

int main()
{
    int a = -42;
    printf("%s", ft_itoa(a));
}