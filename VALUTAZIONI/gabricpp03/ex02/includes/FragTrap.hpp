/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 15:49:27 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRAGTRAP_HPP
#define FRAGTRAP_HPP

#include "ClapTrap.hpp"

/*
** FragTrap Class - Second Derived Class from ClapTrap
**
** Demonstrates that multiple classes can inherit from the same base.
**
** INHERITANCE RELATIONSHIP:
** FragTrap "is-a" ClapTrap (inherits all ClapTrap's attributes and methods)
**
** KEY DIFFERENCES FROM SCAVTRAP:
** 1. Different stats: 100 HP, 100 Energy, 30 Attack (vs ScavTrap's 100/50/20)
** 2. Does NOT override attack() (uses ClapTrap's version)
** 3. Different special ability: highFivesGuys() (vs ScavTrap's guardGate())
**
** INHERITANCE CHOICES:
** - attack(): INHERITS from ClapTrap (no override, unlike ScavTrap)
** - takeDamage(): INHERITS from ClapTrap (same as ScavTrap)
** - beRepaired(): INHERITS from ClapTrap (same as ScavTrap)
** - highFivesGuys(): NEW method (unique to FragTrap)
*/
class	FragTrap : public ClapTrap {
	public:
		FragTrap();
		FragTrap(std::string name);
		FragTrap(const FragTrap& other);
		FragTrap	&operator=(FragTrap const &other);

		/*
		** Virtual Destructor
		** Makes FragTrap's destructor virtual for future extensibility.
		** Marked virtual even though FragTrap won't be a base class in ex02.
		** Good practice: if base has virtual destructor, derived should too.
		**
		** DESTRUCTION ORDER when FragTrap goes out of scope:
		** 1. ~FragTrap() runs first
		**    - Prints: "FragTrap destructor for Robot has been called!"
		** 2. ~ClapTrap() runs automatically (inheritance chain)
		**    - Prints: "ClapTrap Robot has been destroyed!"
		*/
		virtual	~FragTrap();

		/*
		** highFivesGuys() - NEW METHOD
		*/
		void	highFivesGuys(void);
};

#endif
