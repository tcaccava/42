/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 23:44:29 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/19 01:28:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <string.h>

size_t	ft_strspn(const char *s, const char *accept)

{
    size_t i = 0;  
    while (*s)
    {
        const char *a = accept;
        while (*a)
        {
            if (*s == *a)
                break;
            a++;
        }
        if (*a == '\0')
            break;
        s++;
        i++;
    }
    return i;
}

int main()
{
    char test[] = "ciaone";
    char test2[] = "zzciazo";
    printf("%zu\n", ft_strspn(test, test2));
    printf("%zu\n", strspn(test, test2));
    return 0;
}