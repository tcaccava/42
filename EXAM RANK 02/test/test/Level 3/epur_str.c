/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epur_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 04:57:01 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 07:30:02 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

int main(int c, char const **v)
{
	if(c == 2)
    {
        int i = 0;
        int first_word = 0;
        while (v[1][i] == ' ' || v[1][i] == '\t')
            i++;
        while (v[1][i])
        {
            if (v[1][i] != ' ' && v[1][i] != '\t')
                {
                    if(first_word)
                        write(1, " ", 1);
                    while (v[1][i] && v[1][i] != ' ' && v[1][i] != '\t')
                    {
                        write(1, &v[1][i], 1);
                        i++;
                    }
                    first_word = 1;
                }     
                while (v[1][i] && (v[1][i] == ' ' || v[1][i] == '\t')) 
                    i++;          
        }    
    }
    write(1, "\n", 1);
    return 0;
}