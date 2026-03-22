/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_capitalizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:50:38 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 20:57:51 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

void str_capitalizer(char *s)
{
    int i = 0;
    if (s[i] >= 'a' && s[i] <= 'z')
        s[i] = s[i] - 32;
    while (s[i])
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] + 32;
        if ((s[i] >= 'a' && s[i] <= 'z') && (s[i - 1] == ' ' || s[i - 1] == '\t'))
            s[i] =s[i] - 32;
        write(1, &s[i], 1);
        i++;
    }
}

int main(int c, char **v)
{
    int i;
    if (c < 2)
    {
        write(1, "\n", 1);
    }
    else
    {
        int i = 1;
        while(i < c) 
        {
            str_capitalizer(v[i]);
            write(1, "\n", 1);
            i++;
        }
    }
    return 0;
}