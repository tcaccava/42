/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongDog.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:48:21 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 15:42:53 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WrongDog_HPP
#define WrongDog_HPP

#include "WrongAnimal.hpp"

class WrongDog : public WrongAnimal {

	public:
		WrongDog();
		WrongDog( const std::string& name );
		WrongDog ( const WrongDog& WrongDog);

		WrongDog&	operator=( const WrongDog& WrongDog );

		void	makeSound( void ) const;

		~WrongDog();
};

#endif
