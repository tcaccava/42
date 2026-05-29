/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:42:27 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 13:48:24 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAVTRAP_HPP
#define SCAVTRAP_HPP

#include "ClapTrap.hpp"

class ScavTrap: virtual public ClapTrap {

	public:
		ScavTrap();
		ScavTrap( const std::string& newName );
		ScavTrap( const ScavTrap& scavTrap );

		ScavTrap&		operator=( const ScavTrap& scavTrap );

		virtual void	attack( const std::string& target );
		void			guardGate( void );

		virtual ~ScavTrap();
};

#endif
