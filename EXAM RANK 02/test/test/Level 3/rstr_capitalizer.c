/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rstr_capitalizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:24:34 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 19:48:19 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
/*
void capitalize(char *s)
{
    int i = 0;
    while(s[i])
    {
        if ((s[i] >= 'a' && s[i] <= 'z') && (s[i + 1] != ' ' && s[i + 1] != '\t' && s[i + 1] != '\0'))
            write(1, &s[i], 1);
        else if ((s[i] >= 'A' && s[i] <= 'Z') && (s[i + 1] != ' ' && s[i + 1] != '\t' && s[i + 1] != '\0'))
        {
            s[i] = s[i] + 32;
            write(1, &s[i], 1);
        }
        else if ((s[i] >= 'a' && s[i] <= 'z') && (s[i + 1] == ' ' || s[i + 1] == '\t'))
            {
            s[i] = s[i] - 32;
            write(1, &s[i], 1);
            }
        else if (((s[i] >= 'A' && s[i] <= 'Z') && (s[i + 1] == ' ' || s[i + 1] == '\t')))
            write(1, &s[i], 1);
        else if((s[i] >= 'a' && s[i] <= 'z') && s[i + 1] == '\0')
            {
            s[i] = s[i] - 32;
            write(1, &s[i], 1);
            }    
        else
            write(1, &s[i], 1);
        i++;
    }
}
int main(int c, char **v)
{
    if (c == 1)
        write(1 ,"\n", 1);
    else
    {    
        int i = 1;
        while (i <= c - 1)
        {
            capitalize(v[i]);
            write(1, "\n", 1);
            i++;
        }
    }
    return 0;
}
*/
#include <unistd.h>

void    rstr_capitalizer(char *str)
{
    int i = 0;

    while (str[i])
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
        if ((str[i] >= 'a' && str[i] <= 'z') && (str[i + 1] == ' ' || str[i + 1] == '\t' || str[i + 1] == '\0'))
            str[i] -= 32;
        write(1, &str[i], 1);
        i++;
    }
}

int main(int argc, char **argv)
{
    int i;

    if (argc == 1)
        write(1, "\n", 1);
    else
    {
        i = 1;
        while (i < argc)
        {
            rstr_capitalizer(argv[i]);
            write(1, "\n", 1);
            i += 1;
        }
    }
    return (0);
}