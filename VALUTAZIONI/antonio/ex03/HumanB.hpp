/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:57:07 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/09 09:26:56 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANB_HPP
#define HUMANB_HPP

#include "Weapon.hpp"

class HumanB {

private:
	std::string	name;
	Weapon*		weapon;

public:
	HumanB(const std::string& name = "null");

	const std::string&	getName( void ) const { return (name); }
	void				setName ( const std::string& newName ) { name = newName; }
	void				setWeapon ( Weapon& newWeapon ) { weapon = &newWeapon; }

	void	attack( void );

	~HumanB();
};

#endif
