/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ice.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:37:27 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:18:39 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICE_HPP
#define ICE_HPP

#include "AMateria.hpp"
#include <iostream>

class Ice: public AMateria 
{
    public:
        Ice();
        Ice(const Ice &instance);
        Ice& operator=(const Ice &instance);
        ~Ice();

        AMateria* clone() const;
        void use(ICharacter& target);
};

#endif