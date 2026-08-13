/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 20:48:15 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/19 21:50:20 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        int i = 0;
        while(argv[1][i])
            i++;
        i--;
        while (i >= 0 && (argv[1][i] == 32 || argv[1][i] == 9))
            i--;
        int lastword_end = i;
        while (i >= 0 && (argv[1][i] != 9 && argv[1][i] != 32))
            i--;
        i++; //serve per partire dal primo carattere dell' ultima parola e non dallo spazio precedente
        while (i <= lastword_end)
        {
            write(1, &argv[1][i], 1);
            i++;
        }
    }
    write(1, "\n", 1);
    return 0;
}   
 