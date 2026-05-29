/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:57:03 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/04 16:57:04 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <iostream>

class Weapon {

private:
	std::string type;

public:
	Weapon(const std::string& newType = "null");

	const std::string&	getType( void ) const { return (type); }
	void				setType( const std::string& newType ) { type = newType; }

	~Weapon();

};

#endif

