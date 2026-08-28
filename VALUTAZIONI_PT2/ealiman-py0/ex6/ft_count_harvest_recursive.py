# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    ft_count_harvest_recursive.py                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ealiman <ealiman@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/03 17:57:48 by ealiman           #+#    #+#              #
#    Updated: 2026/08/03 18:19:42 by ealiman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

def call_recursive(n):
    if n <= 0:
        return
    else:
        call_recursive(n - 1)
        print("Day ", n)


def ft_count_harvest_recursive():
    days = int(input("Days until harvest: "))
    call_recursive(days)
    print("Harvest time!")
