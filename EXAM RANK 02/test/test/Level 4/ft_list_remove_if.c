/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_remove_if.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 04:48:01 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 05:12:39 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ft_list.h"
#include <stdlib.h>

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
    if (begin_list == NULL || *begin_list == NULL)
		return;
    t_list *current = *begin_list;
    if (cmp(current->data, data_ref) == 0)
    {
     *begin_list = current->next;
     free(current);
     ft_list_remove_if(begin_list, data_ref, cmp);
    }
    else
    {
        current = *begin_list;
        ft_list_remove_if(&current->next, data_ref, cmp);
    }   
}