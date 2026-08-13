/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rew_wstr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 06:23:03 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 10:43:49 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
/*
int word_length(char *str)
{
    int len = 0;
    while (*str && *str != ' ' && *str != '\t')
    { 
        len++;
        str++;
    }
    return len;
}
int word_count(char *str)
{
    int count = 0;
    while (*str)
    {
        while (*str == ' ' || *str == '\t')
            str++;
        if (*str)
        {
            count++;
            while (*str && *str != ' ' && *str != '\t')
                str++;
        }
    }
    return count;
}   

void rew_wstr( char *str)
{

    int words = word_count(str);
    char **matrix = malloc(sizeof(char *) * words);
    if (!matrix)
        return;
    int i = 0;
    int word_index = 0;
    while(str[i])
    {
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        if (str[i])
        {   
            matrix[word_index] = &str[i];
            word_index++;
            int length = word_length(&str[i]);
            i = i + length;
        }
    }
    int index = words - 1;
    while (index >= 0)
    {
        write(1, matrix[index], word_length(matrix[index]));
        if (index > 0)
            write(1, " ", 1);
        index--;
    }
    free(matrix);
}
int main(int c, char **v)
{
    if (c == 2)
        rew_wstr(v[1]);
    write(1, "\n", 1);  
}
*/
#include <unistd.h>
void rew_wstr(char *str)
{
    int i = 0;
    while (str[i] && (str[i] == ' ' || str[i] == '\t'))
        i++;
    int first = i;
    while (str[i])
        i++;
    while (i >= first)
    {
        while (i >= 0 && (str[i] == ' ' || str[i] == '\t'))
            i--;
        int end = i;   
        while (i >= 0 && (str[i] != ' ' && str[i] != '\t'))
            i--;
        int start = i + 1;
        int flag = start;
        while (start <= end)
        {
            write(1, &str[start], 1);
            start++;
        }
        if(flag != first)
            write(1, " ", 1);
    } 
}

int main(int c, char **v)
{
    if (c == 2)
        rew_wstr(v[1]);
    write (1, "\n", 1);
}
