/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpbrk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 22:44:57 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/18 23:01:46 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

char	*ft_strpbrk(const char *s1, const char *s2)
{
    while (*s1)
    {
        const char *tmp = s2;
        while(*tmp)
        {
            if (*tmp == *s1)
                return (char *)s1;
            tmp++;          
        }
        s1++;
    }
    return (NULL);
}
int main()
{
    char test[] = "Ciaone";
    char test2[] = "wxyzo";
    printf("%s", ft_strpbrk(test, test2));
    return 0;
}