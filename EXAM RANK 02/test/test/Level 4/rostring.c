/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rostring.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 09:57:07 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 12:06:07 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include <stdlib.h>

int len(char *s)
{
    int len = 0;
    while (*s && *s != '\t' && *s != ' ')
    {
        s++;
        len++;
    }
    return len;
}
int count_words(char *s)
{
    int count = 0;
    while (*s)
    {
        while(*s == ' ' || *s == '\t')
            s++;
        if(*s)
        {
            count++;
            while(*s && *s != ' ' && *s != '\t')
                s++;
        }
    }
    return count;
}

void rostring(char *s)
{
 int words = count_words(s);
 char **matrix = malloc(sizeof(char *) * words);
 int wordindex = 0;
 int i = 0;
 while (s[i])
 {
    while (s[i] == ' ' || s[i] == '\t')
        i++;
    if(s[i])
        {
            matrix[wordindex] = &s[i];
            wordindex++;
            int length = len(&s[i]);
            i = i + length;
        }
 } 
int j = 1;
while (j < words)
{
    write(1, matrix[j], len(matrix[j]));
    write(1, " ", 1);
    j++;
}    
write(1, matrix[0], len(matrix[0]));
free(matrix);
}

int main(int c, char **v)//ci sono dubbi sulla gestione di argc,il subject non e molto chiaro con gli esercizi
{
    if (c == 2)
    {
        rostring(v[1]);
    }
    else if (c > 2)
    {
        write(1, v[1], len(v[1]));
    }
    write(1, "\n", 1);
    return 0;
}