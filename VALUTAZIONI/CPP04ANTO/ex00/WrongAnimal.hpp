/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:37:37 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 15:42:53 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRONGANIMAL_HPP
#define WRONGANIMAL_HPP

#include <iostream>

class WrongAnimal {

	protected:
		std::string	name;
		std::string	type;

	public:
		WrongAnimal();
		WrongAnimal( const std::string& newName );
		WrongAnimal ( const WrongAnimal& wrongAnimal );

		WrongAnimal&	operator=( const WrongAnimal& wrongAnimal );

		inline const std::string&	getName( void ) const { return (name); }
		inline const std::string&	getType( void ) const { return (type); }
		inline void					setName( const std::string& newName ) { name = (newName == "") ? name : newName; }
		inline void 				setType( const std::string& newType ) { type = (newType == "") ? type: newType; }

		void	makeSound( void ) const;

		~WrongAnimal();
};

#endif
