/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 16:35:58 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 16:36:01 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

#include <string>

class Zombie
{
    private:
        std::string _name;
        
    public:
        Zombie();
        Zombie(std::string name);
        ~Zombie();
        void setName(std::string name);
        void announce(void);
};

#endif
