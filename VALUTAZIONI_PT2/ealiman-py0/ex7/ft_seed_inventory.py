# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    ft_seed_inventory.py                               :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ealiman <ealiman@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/03 19:01:43 by ealiman           #+#    #+#              #
#    Updated: 2026/08/03 19:41:48 by ealiman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

def ft_seed_inventory(seed_type: str, quantity: int, unit: str) -> None:
    if unit == 'packets':
        print(f"{seed_type.capitalize()} seed: {quantity} packets availble")
    elif unit == 'grams':
        print(f"{seed_type.capitalize()} seed: {quantity} grams total")
    elif unit == 'area':
        print(f"{seed_type.capitalize()} seed: covers {quantity} square meters")
    else:
        print("Unknow unit type")
