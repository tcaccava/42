/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_bits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 23:53:19 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 01:07:45 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <unistd.h>
unsigned char	reverse_bits(unsigned char octet)
{
    unsigned char reversed = 0;
    int i = 8;
    while (i > 0)
    {
        reversed = reversed << 1;
        reversed = reversed | (octet & 1);
        octet = octet >> 1;
        i--;
    }
    return reversed;
}
void	print_bits(unsigned char octet)
{
    unsigned char bit;
    int i = 7;
    while (i >= 0)
    {
        bit = (octet >> i & 1) + '0';
        write(1, &bit, 1);
        i--;
    }
}
int main()
{
    unsigned char test = 0b00000010;
    print_bits(reverse_bits(test));
    return 0;
}
