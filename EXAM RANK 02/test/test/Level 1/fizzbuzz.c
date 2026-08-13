/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fizzbuzz.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:50:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/17 12:51:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void putnbr(int n)
{
    char c;
    if (n > 9)
        putnbr(n / 10);
    c = (n % 10) + '0';
    write(1, &c, 1);
}

void fizzbuzz()
{
    int c = 1;
    while (c <= 100)
    {
        if (c % 5 == 0 && c % 3 == 0)
            write(1, "fizzBuzz", 8);
        else if (c % 5 == 0)
            write(1, "fizz", 4);
        else if (c % 3 == 0)
            write(1, "buzz", 4);
        else
        {
        putnbr(c);
        }
        write (1, "\n", 1);
    c++;
    }
}

int main()
{
    fizzbuzz();
    return 0;
}