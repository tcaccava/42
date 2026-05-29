/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Character.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:14:30 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:30:03 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "ICharacter.hpp"
#include "AMateria.hpp"

#define MAX_MATERIAS 4

class Character : public ICharacter {

	private:
		std::string	name;
		AMateria*	materias[MAX_MATERIAS];
	public:
		Character();
		Character( const std::string& newName );
		Character( const Character& character );

		Character&	operator=( const Character& character );

		virtual std::string const & getName() const;
		virtual void equip(AMateria* m);
		virtual void unequip(int idx);
		virtual void use(int idx, ICharacter& target);

		~Character();
};

#endif
