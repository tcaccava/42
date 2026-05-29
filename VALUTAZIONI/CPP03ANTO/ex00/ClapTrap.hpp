/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:24:44 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 09:34:47 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <iostream>

const int	HIT_POINTS = 10;
const int	ENERGY_POINTS = 10;
const int	ATTACK_DAMAGE = 0;

class ClapTrap {

	private:
		std::string		name;
		unsigned int	hitPoints;
		unsigned int	energyPoints;
		unsigned int	attackDamage;

	public:
		ClapTrap();
		ClapTrap( const std::string& newName );
		ClapTrap( const ClapTrap& clapTrap );

		ClapTrap&							operator=( const ClapTrap& clapTrap );

		const std::string&					getName( void ) const;
		const unsigned int&					getHitPoints( void ) const;
		const unsigned int&					getEnergyPoints( void ) const;
		const unsigned int&					getAttackDamage( void ) const;

		void								setName( const std::string& newName );
		void								setHitPoints( const unsigned int& newHitPoints );
		void								setEnergyPoints( const unsigned int& newEnergyPoints );
		void								setAttackDamage( const unsigned int& newAttackDamage );

		void								attack( const std::string& target );
		void								takeDamage( unsigned int amount );
		void								beRepaired( unsigned int amount );

		~ClapTrap();
};

#endif
