/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMateria.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:14:25 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:30:03 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMATERIA_HPP
#define AMATERIA_HPP

#include <iostream>
#include "ICharacter.hpp"

class AMateria {

	protected:
		std::string	type;

	public:
		AMateria();
		AMateria( std::string const & type );
		AMateria( const AMateria& aMateria );

		AMateria&	operator=( const AMateria& aMateria );

		std::string const& 	getType() const;
		virtual AMateria*	clone() const = 0;
		virtual void		use(ICharacter& target);

		virtual ~AMateria();

};

#endif
