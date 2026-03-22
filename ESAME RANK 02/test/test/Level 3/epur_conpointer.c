/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epur.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 07:43:51 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 07:56:37 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

void epur (char *s)
{
    int firstword = 0;
    while (*s == ' ' || *s == '\t')
        s++;
    while (*s)
    {
        if (*s != ' ' && *s != '\t')
        {
            if (firstword)
                write(1, " ", 1);
            while (*s && *s != ' ' && *s != '\t')
            {
                write (1, s, 1);
                s++;
            }
            firstword = 1;
        }
        while (*s && *s == ' ' || *s == '\t')
            s++;
    }
}
int main(int c, char **v)
{
    if(c == 2)
        epur(v[1]);
    write(1, "\n", 1);
    return 0;
}