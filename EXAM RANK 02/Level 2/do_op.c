/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*         do_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:50:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/17 12:51:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        int a = atoi(argv[1]);
        int b = atoi(argv[3]);
        char c = argv[2][0];
        if (c == '+')
            printf("%d\n", a + b );
        else if(c == '-')
            printf("%d\n", a - b);
        else if (c == '*')
            printf("%d\n", a * b);
        else if(c == '/')
            printf("%d\n", a / b);
        else if (c == '%')
            printf("%d\n", a % b);
    }
    else
        printf("\n");
    return 0;
}
