/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_mult.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 21:04:11 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 21:45:10 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

int ft_atoi(char *s)
{
    int result = 0;
    int sign = 1;
    while (*s == ' ' || (*s >= 9 && *s <= 13))
        s++;
    if (*s == '+')
        s++;
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s - '0');
        s++;
    }
    return (result * sign);
}

void ft_putnbr(int n)
{
    if (n > 9)
        ft_putnbr(n / 10);
    char digit = n % 10 + '0';
    write(1, &digit, 1);    
}
int main (int c,char **v)
{
    if (c != 2)
        write(1, "\n", 1);
    else
    {
        int nbr;
        int i = 1;
        nbr = ft_atoi(v[1]);
        while(i <= 9)
        {
            ft_putnbr(i);
            write(1, " x ", 3);
            ft_putnbr(nbr);
            write(1," = ", 3);
            ft_putnbr(i * nbr);
            write(1, "\n", 1);
            i++;
        }
    }
        return 0;
}