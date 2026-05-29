/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cure.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:14:39 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:14:40 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CURE_HPP
#define CURE_HPP

#include "AMateria.hpp"

class Cure : public AMateria {

	public:
		Cure();
		Cure(const Cure& cure);

		Cure& operator=(const Cure& cure);

		virtual AMateria* clone() const;
		virtual void use(ICharacter& target);

		~Cure();

};

#endif
