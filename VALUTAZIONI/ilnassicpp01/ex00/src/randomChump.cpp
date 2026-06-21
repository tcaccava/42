/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomChump.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 16:55:43 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 16:55:44 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

/*Creates a Zombie on the STACK (local/automatic storage) 
and makes it announce itself.
Since 'zumba' is a stack variable, it is automatically destroyed
when the function returns (no manual 'delete' needed).*/
void randomChump(std::string name)
{
    Zombie zumba(name);
    zumba.announce();
}
