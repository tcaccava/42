/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 01:20:24 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/31 02:08:12 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANA_HPP
# define HUMANA_HPP

#include <iostream>
#include <string>
#include "Weapon.hpp"

class   HumanA
{
    private:
        std::string _name;
        Weapon&     _weaponREF;

    public:
        HumanA(std::string name, Weapon& weapon);
        ~HumanA();
        void attack() const;
};

#endif
