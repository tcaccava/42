/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*     ft_atrcmp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:50:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/17 12:51:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int    ft_strcmp(char *s1, char *s2)
{
    while (*s1 && *s2)
    {
        if(*s1 != *s2)
            return (*s1 - *s2);
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}  
int main()
{
    char test[] = "Helloz";
    char test2[] = "Helloa";
    printf("%i", ft_strcmp(test, test2));
}