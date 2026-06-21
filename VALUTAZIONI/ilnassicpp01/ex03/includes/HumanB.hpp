/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 02:09:24 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/31 02:14:46 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANB_HPP
# define HUMANB_HPP

#include <string>
#include "Weapon.hpp"

class HumanB
{
    private:
        std::string _name;
        Weapon*     _weaponPTR;

    public:
        HumanB(std::string name);
        ~HumanB();
        void attack() const;
        void setWeapon(Weapon& weapon);
};

#endif
