/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ice.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:14:17 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:14:18 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICE_HPP
#define ICE_HPP

#include "AMateria.hpp"

class Ice : public AMateria {

	public:
		Ice();
		Ice( const Ice& ice );

		Ice&	operator=( const Ice& ice );

		virtual AMateria* clone() const;
		virtual void use(ICharacter& target);

		~Ice();

};

#endif
