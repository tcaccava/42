/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newZombie.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 16:55:30 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 16:55:34 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

/*Allocates a Zombie on the HEAP and returns a pointer to it.
Caller is responsible for calling 'delete' on the returned pointer
to avoid a memory leak (the object will NOT be destroyed automatically).*/
Zombie* newZombie(std::string name)
{
    return new Zombie(name);
}
