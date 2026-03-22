/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_prime_sum.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 03:37:03 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 04:28:27 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

int is_prime(int num)
{
    if (num <= 1)
        return 0;
    if (num == 2)
        return 0;
    if (num % 2 == 0)
        return 0;
    int i = 2;
    while (i * i <= num)
        {
            if (num % i == 0)
                return 0;
            i++;
        }
    return 1;
}

void ft_putnbr(int n)
{
    if(n >= 10)    
        ft_putnbr(n / 10);
    char digit = (n % 10) + '0';
    write(1 , &digit, 1);
}

int ft_atoi(char *s)
{
    int result = 0;
    while (*s)
    {
        result = (result * 10) + (*s - '0');
        s++;
    }
    return (result);
}
int main(int argc, char **argv)
{
    if (argc == 2)
    {
        int num = ft_atoi(argv[1]);
        int sum = 0;
        while (num > 0)
        {
            if(is_prime(num))
                sum = sum + num;
            num--;
        }
        ft_putnbr(sum);
    }
    if (argc != 2)
        ft_putnbr(0);
    write(1, "\n", 1);
    return 0;
}