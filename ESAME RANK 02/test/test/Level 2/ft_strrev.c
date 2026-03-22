/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 23:07:07 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/18 23:40:17 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

char *ft_strrev(char *str)
{
    int len = 0;
    char *tmp = str;
    while (*tmp)
    {
        len++;
        tmp++;
    }
    len--;
    int i = 0;
    char temp;
    while (i <= len)
    {
        temp = str[i]; 
        str[i] = str[len];
        str[len] = temp;
        i++;
        len--;
    }
    return str;
}

int main()
{
    char test[] = "Tobia";
    printf("%s", ft_strrev(test));
    return 0;
}