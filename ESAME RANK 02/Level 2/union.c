/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   union.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 02:03:30 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 02:28:28 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

int seen(char *s, char c, int index)
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

void uni (char *s1, char *s2)
{
    int i = 0;
    while (s1[i])
    {
        if (!seen(s1,s1[i],i))
            write(1, &s1[i], 1);
        i++;      
    }
    int j = 0;
    while (s2[j])
    {
        if((!seen(s2,s2[j],j)) && (!seen(s1,s2[j],i)))
            write(1, &s2[j], 1);
        j++;    
    }
}

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        uni(argv[1],argv[2]);
    }
    write(1, "\n", 1);
}