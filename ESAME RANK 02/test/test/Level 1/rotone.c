/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotone.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:50:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/17 12:51:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void rotone(char *s)
{
    while (*s)
    {
        if (*s >= 'a' && *s <= 'y' || *s >= 'A' && *s <= 'Y')
            {
            *s = *s + 1;
            write(1, s, 1);
            }
        else if (*s == 'z' || *s == 'Z')
            {
            *s = *s - 25;
            write(1, s, 1);
            }
        else
            write(1, s, 1);
    s++;
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        rotone(argv[1]);
        write(1, "\n", 1);
    }
    else
        write(1, "\n", 1);
    return 0;
}