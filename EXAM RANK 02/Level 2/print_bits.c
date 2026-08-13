/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 22:51:28 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/19 23:51:14 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

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
    print_bits(23);
    return 0;
}