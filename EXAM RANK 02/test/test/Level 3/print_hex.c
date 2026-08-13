/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:40:33 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 18:01:57 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

int ft_atoi(char *s)
{
    int result = 0;
    while (*s)
    {
        result = result * 10 +(*s - '0');
        s++;
    }
    return (result);
}

void put_hex(int n)
{
    char hex[] = "0123456789abcdef";
    if (n >= 16)
        put_hex(n / 16);
    write(1, &hex[n % 16], 1);
}

int main(int c, char **v)
{
    if (c == 2)
        put_hex(ft_atoi(v[1]));
    write(1, "\n", 1);
}