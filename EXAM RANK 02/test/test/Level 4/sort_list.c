/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:29:31 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/22 15:39:06 by tcaccava         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "list.h"

t_list	*sort_list(t_list* lst, int (*cmp)(int, int))
{
    t_list *start;
    start = lst;
    int swap;
    while (lst != NULL && lst->next != NULL)
    {
        if ((*cmp)(lst->data, lst->next->data) == 0);
        {
            swap = lst->data;
            lst->data = lst->next->data;
            lst->next->data = swap;
            lst = start;
        }
        else
            lst = lst->next;
    }
    return start;
}