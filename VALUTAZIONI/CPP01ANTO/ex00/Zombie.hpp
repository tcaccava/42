/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:12:29 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 14:12:30 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include <iostream>

class Zombie {

private:
	std::string	name;

public:
	Zombie( std::string zombieName = "null" );
	void	announce( void );
	~Zombie();
};

Zombie*	newZombie( std::string name );
void	randomChump( std::string name );

#endif
