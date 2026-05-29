/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiamondTrap.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:44:33 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 13:54:19 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIAMONDTRAP_HPP
#define DIAMONDTRAP_HPP

#include "ScavTrap.hpp"
#include "FragTrap.hpp"

class DiamondTrap : public FragTrap, public ScavTrap {

	private:
		std::string	name;

	public:
		DiamondTrap();
		DiamondTrap( const std::string name );
		DiamondTrap( const DiamondTrap& diamondTrap );

		DiamondTrap&	operator=( const DiamondTrap& diamondTrap );

		void			whoAmI( void );
		virtual void	attack( const std::string& target );

		~DiamondTrap();
};

#endif
