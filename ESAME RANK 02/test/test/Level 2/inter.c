/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:11:36 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/19 20:28:01 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

int already_seen(char *s, char c, int index)
{
    int i = 0;
    while (i < index)
    {
        if (s[i] == c)
            return 1;
        i++;    
    }
    return 0;
}

void inter (char *s1, char *s2)
{
    int i = 0;
    while (s1[i])
    {
        if (!already_seen(s1, s1[i], i))
        {
            char *tmp = s2;
            while (*tmp)
            {
                if (s1[i] == *tmp)
                {    
                    write (1, &s1[i], 1);
                    break;
                }
            tmp++;    
            }
        }
    i++;
    }
}
int main(int argc, char **argv)
{
    if (argc == 3)
        inter(argv[1], argv[2]);
    write(1, "\n", 1);
    return 0;
}