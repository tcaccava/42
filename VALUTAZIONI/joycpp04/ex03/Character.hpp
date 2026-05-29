/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Character.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:37:03 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:18:23 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "ICharacter.hpp"
#include <iostream>
#include <string>

class Character: public ICharacter 
{
    private:
        std::string name;
        AMateria *inventory[4];
        AMateria *floor[4];
    public:
        Character();
        Character(std::string Name);
        Character(const Character &instance);
        Character& operator=(const Character &instance);
        ~Character();
        
        std::string const & getName() const;
        
        void equip(AMateria* m);
        void unequip(int idx);
        void use(int idx, ICharacter& target);
};

#endif