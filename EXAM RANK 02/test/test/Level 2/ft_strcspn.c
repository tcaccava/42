/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 20:42:45 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/19 00:09:19 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include <stdio.h>

size_t	ft_strcspn(const char *s, const char *reject)
{
    size_t i = 0;
    while (*s)
    {
        const char *r = reject;
        while (*r)
        {
            if (*s == *r)
                return i;
            r++;
        }
        i++;
        s++;
    }
    return i;
}
int main()
{
    char test[] = "hello, world";
    char test1[] = "zwo";
    printf("%zu\n", ft_strcspn(test, test1));
    return 0;
}