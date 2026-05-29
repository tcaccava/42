/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:42:27 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/10 16:26:26 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAVTRAP_HPP
#define SCAVTRAP_HPP

#include "ClapTrap.hpp"

class ScavTrap: public ClapTrap {

	public:
		ScavTrap();
		ScavTrap( const std::string& newName );
		ScavTrap( const ScavTrap& scavTrap );

		ScavTrap&		operator=( const ScavTrap& scavTrap );

		virtual void	attack( const std::string& target );
		void			guardGate( void );

		~ScavTrap();
};

#endif
