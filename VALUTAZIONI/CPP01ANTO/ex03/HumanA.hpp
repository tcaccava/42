/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:57:04 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/09 09:25:36 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANA_HPP
#define HUMANA_HPP

#include "Weapon.hpp"

class HumanA {

private:
	std::string	name;
	Weapon&		weapon;

public:
	HumanA(const std::string& name, Weapon& newWeapon);

	const std::string&	getName( void ) const { return (name); }
	void				setName ( const std::string& newName ) { name = newName; }

	void				attack( void );

	~HumanA();

};

#endif
