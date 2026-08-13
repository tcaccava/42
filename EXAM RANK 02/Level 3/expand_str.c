/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:01:49 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 08:22:16 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

void expand(char *s)
{
    while (*s == ' ' || *s == '\t')
        s++;
    int firstword = 0;
    while(*s)
    {
        if (*s != ' ' && *s != '\t')
        {
            if(firstword)
                write(1,"   ", 3);
            while (*s && (*s != ' ' && *s != '\t'))
            {   
            write(1, s, 1);
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
    if (c == 2)
        expand(v[1]);
    write(1,"\n", 1);
    return 0;
}