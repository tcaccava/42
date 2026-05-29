/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:42:14 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 14:42:15 by apintaur         ###   ########.fr       */
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
	void		setName( std::string zombieName ) { name = zombieName; }
	std::string	getName( void ) { return name; }
	void		announce( void );
	~Zombie();
};

Zombie*	zombieHorde( int N, std::string name );
void	randomChump( std::string name );

#endif
