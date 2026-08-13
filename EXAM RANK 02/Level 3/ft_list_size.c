/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 09:55:06 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/20 10:25:53 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ft_list.h"

int	ft_list_size(t_list *begin_list)
{
    int i = 0;
    while (begin_list)
    {
        begin_list = begin_list->next;
        i++;
    }
    return i;
}