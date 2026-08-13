/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:50:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/17 12:51:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void search_word(char *s)
{
    while(*s == 9 || *s == 32)
        s++;
    while (*s && *s != 9 && *s != 32)
    {
        write(1, s, 1);
        s++;
    }
}

int main(int argc, char **argv)
{
    if(argc == 2)
        search_word(argv[1]);
    write(1, "\n", 1);
    return(0);
}