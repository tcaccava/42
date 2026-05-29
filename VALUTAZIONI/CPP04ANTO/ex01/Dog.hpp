/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:48:21 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:25:47 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOG_HPP
#define DOG_HPP

#include "Animal.hpp"
#include "Brain.hpp"

class Dog : public Animal {

	private:
		Brain*	brain;

	public:
		Dog();
		Dog ( const Dog& dog);

		Dog&	operator=( const Dog& dog );

		virtual void	makeSound( void ) const;

		~Dog();
};

#endif
