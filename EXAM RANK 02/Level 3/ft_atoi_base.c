/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:31:13 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 09:18:55 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

int	ft_atoi_base(const char *str, int str_base)
{
	int sign = 1;
    int result = 0;

    
    if (*str == '-')
    {
        sign = sign * -1;
        str++;
    }
    int i = 0;
    int c;
    while (str[i])
    {
        if (str[i] >= '0' && str[i] <= '9')
            c = str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'f')
            c = str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F')
            c = str[i] - 'A' + 10;
        else
            break;
        result = result * str_base + c;
        i++;
    }
    return (result * sign);
}