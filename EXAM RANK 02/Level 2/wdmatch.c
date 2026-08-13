/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wdmatch.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 02:34:53 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 03:32:49 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

void wdmatch(char *s1, char *s2)
{
    char *temp = s1;
    while (*s2)
    {
        if (*s2 == *temp)
            temp++;
        s2++;
    }
    if (*temp == '\0')
    {
        while (*s1)
            {
                write(1, s1, 1);
                s1++;
            }
    }
}
int main(int argc, char **argv)
{    
    if(argc == 3)
    {
        wdmatch(argv[1], argv[2]);
    }
    write(1, "\n", 1);
    return 0;
}
