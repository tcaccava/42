/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 20:25:45 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 00:24:05 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "flood_fill.h"

void fill(char **matrix, t_point size, t_point pos, char target)
{
    if (pos.x >= size.x || pos.y >= size.y || pos.y < 0 || pos.x < 0)
        return;
    if (matrix[pos.y][pos.x] != target)
        return;
    matrix[pos.y][pos.x] = 'F';
    fill (matrix,size, (t_point){pos.x + 1, pos.y},target);
    fill (matrix,size, (t_point){pos.x - 1, pos.y},target);
    fill (matrix,size, (t_point){pos.x, pos. y + 1},target);
    fill (matrix,size, (t_point){pos.x, pos.y - 1},target);       
}

void flood_fill(char **matrix, t_point size, t_point begin)
{
    char target = matrix[begin.y][begin.x];
    if (target == 'F')
        return;
    fill(matrix, size,  begin, target);
}
    
char** newmatrix(char **old, t_point size)
{
    char **new;
    new = malloc(sizeof(char *) * size.y);
    if (!new)
        return (NULL);
    int i = 0;
    while (i < size.y)
    {
        new[i] = malloc(size.x + 1);
        int j =0;
        while(j < size.x)
        {
            new[i][j] = old[i][j];
            j++; 
        }
        new[i][size.x] = '\0';
        i++;
    }  
    return new;
}

int main(void)
{
     t_point size = {8, 5};
     char* old[] = {
        "11111111",
        "10001001",
        "10010001",
        "10110001",
        "11100001",
     };
     char **new = newmatrix(old, size);
     int i = 0;
     while (i < size.y)
     {
        printf("%s\n", new[i]);
        i++;
     }
     printf("\n");
     t_point begin = {7, 4};
     flood_fill(new, size, begin);
     int j = 0;
     while (j < size.y)
        {
            printf("%s\n", new[j]);
            j++;
        }
    return 0;
}

