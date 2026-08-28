# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    ft_harvest_total.py                                :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ealiman <ealiman@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/03 17:17:54 by ealiman           #+#    #+#              #
#    Updated: 2026/08/03 17:20:52 by ealiman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

def ft_harvest_total():
    day1 = int(input("Day 1 harvest total: "))
    day2 = int(input("Day 2 harvest total: "))
    day3 = int(input("Day 3 harvest total: "))
    total = day1 + day2 + day3
    print("Total harvest: ", total)
