/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 22:26:54 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/18 22:43:39 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

char *ft_strdup(char *src)
{
    int i = 0;
    char *dup;
    while (src[i])
    {   
        i++;
    }
    dup = malloc(sizeof(char) * (i + 1));
    if (!dup)
        return (NULL);
    i = 0;
    while (src[i])
    {
        dup[i] = src[i];
        i++;
    }
    dup[i] = '\0';
    return dup;
}
int main()
{
    char test[] = "123t@#";
   printf("%s", ft_strdup(test));
}