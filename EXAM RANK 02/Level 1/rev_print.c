/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rev_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:50:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/17 12:51:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

char *rev_print(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    while (i >= 0)
    {    
        write(1, &str[i], 1);
        i--;
    }
    write(1, "\n", 1);
    return (str);
}
int main()
{
    char test[] = "Ciaone";
    rev_print(test);
    return (0);
}