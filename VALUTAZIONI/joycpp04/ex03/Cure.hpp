/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cure.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:37:16 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:18:31 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CURE_HPP
#define CURE_HPP

#include "AMateria.hpp"
#include <iostream>

class Cure: public AMateria 
{
    public:
        Cure();
        Cure(const Cure &instance);
        Cure& operator=(const Cure &instance);
        ~Cure();

        AMateria* clone() const;
        void use(ICharacter& target);
};

#endif