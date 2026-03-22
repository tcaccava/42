/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repeat_alpha.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:50:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/17 12:51:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void repeat_alpha(char *s)
{
    int i = 0;
    int j;    
    while (s[i])
    {
        if(s[i] >= 65 && s[i] <= 90)
        {
            j = s[i] - 65 + 1;
        }
        else if (s[i] >= 97 && s[i] <= 122)
        {
            j = s[i] - 97 + 1;
        }
        else
        {
            j = 1;
        }
        while(j > 0)
        {
            write(1, &s[i], 1);
            j--;
        }
    i++;
    }   
}

int main(int argc, char **argv)
{   
    if (argc == 2)
        repeat_alpha(argv[1]);
    write(1, "\n", 1);
    return 0;
}
