/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:25 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 16:02:40 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <iostream>

/*
** ClapTrap Class - Base Class for Multiple Derived Classes
**
** In ex02, ClapTrap is the base class for:
** - ScavTrap (from ex01)
** - FragTrap (new in ex02)
**
** INHERITANCE HIERARCHY:
**      ClapTrap (base)
**       /    \
**  ScavTrap  FragTrap (both derived classes)
**
** This demonstrates:
** 1. Multiple classes deriving from the same base
** 2. Different derived classes making different choices about overriding methods
** 3. Code reuse through inheritance (both use ClapTrap's takeDamage/beRepaired)
*/
class ClapTrap {
	/*
	** Protected Attributes
	** Accessible by ClapTrap and ALL its derived classes.
	**
	** PROTECTED ACCESS in ex02:
	** - ClapTrap can access these directly
	** - ScavTrap can access these directly (needs to set HP=100, Energy=50, Attack=20)
	** - FragTrap can access these directly (needs to set HP=100, Energy=100, Attack=30)
	**
	** Why protected?
	** Both derived classes need to initialize these with different values.
	*/
	protected:
		std::string			_name;
		unsigned int		_hitPoints;
		unsigned int		_energyPoints;
		unsigned int		_attackDamage;


	public:
		ClapTrap();
		ClapTrap(std::string name);
		ClapTrap(const ClapTrap& other);
		ClapTrap	&operator=(ClapTrap const &other);
		virtual	~ClapTrap();
		/*
		** Public Methods
		** Define ClapTrap's behavior.
		**
		** METHOD INHERITANCE in ex02:
		** - attack(): ScavTrap OVERRIDES, FragTrap INHERITS (uses ClapTrap's version)
		** - takeDamage(): Both ScavTrap and FragTrap INHERIT (no override)
		** - beRepaired(): Both ScavTrap and FragTrap INHERIT (no override)
		*/
		void	attack(const std::string &target);
		void	takeDamage(unsigned int amount);
		void	beRepaired(unsigned int amount);
};

#endif
