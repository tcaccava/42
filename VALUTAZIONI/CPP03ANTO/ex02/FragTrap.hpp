/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:19:43 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 09:38:56 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRAGTRAP_HPP
#define FRAGTRAP_HPP

#include "ClapTrap.hpp"

class FragTrap : public ClapTrap {

	public:
		FragTrap();
		FragTrap( const std::string& newName );
		FragTrap( const FragTrap& fragTrap );

		FragTrap&		operator=( const FragTrap& fragTrap );

		void			highFivesGuys( void );
		virtual void	attack( const std::string& target );
		~FragTrap();
};

#endif
