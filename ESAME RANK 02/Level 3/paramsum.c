/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paramsum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:58:44 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 16:35:34 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

void ft_putnbr(int n)
{
    if (n > 9)
        ft_putnbr(n / 10);
    char digit = (n % 10) + '0';
    write (1, &digit, 1);
}

int main(int c, char **v)
{
    (void)v; //serve per evitare che compilando con i flag WWW dia unused parameter
    ft_putnbr(c - 1);
    write(1, "\n", 1);
}