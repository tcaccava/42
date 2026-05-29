/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:25 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 16:14:50 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <iostream>

/*
** ClapTrap Class - Base Class for Inheritance
** 
** This is the base class in an inheritance hierarchy:
** ClapTrap (base) -> ScavTrap (derived) -> ...
**
** Key changes from ex00:
** 1. Attributes are now PROTECTED (not private)
** 2. Destructor is now VIRTUAL
**
** These changes are necessary for proper inheritance.
*/
class	ClapTrap {
	/*
	** PROTECTED vs PRIVATE:
	** - Private: Only accessible within ClapTrap class
	** - Protected: Accessible within ClapTrap AND its derived classes
	**
	** Why protected?
	** Derived classes (ScavTrap, etc.) need direct access
	** to these attributes to initialize and modify them.
	** Without protected access, derived classes would need getters/setters.*/
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
		/*
		** CRITICAL: The 'virtual' keyword is essential for inheritance!
		**
		** Why virtual?
		** When you delete a derived class through a base class pointer:
		**   ClapTrap* ptr = new ScavTrap("Robot");
		**   delete ptr;  // Without virtual: only ~ClapTrap() called (MEMORY LEAK!)
		**                // With virtual: ~ScavTrap() then ~ClapTrap() (CORRECT!)
		** Virtual ensures proper destructor chain in inheritance hierarchies.
		** Destructor order: Derived -> Base (reverse of construction order)
		*/
		virtual	~ClapTrap();

		/*
		** Public Methods
		** These define ClapTrap's behavior.
		** Can be inherited (used as-is) or overridden by derived classes.
		*/
		void	attack(const std::string &target);
		void	takeDamage(unsigned int amount);
		void	beRepaired(unsigned int amount);
};

#endif
