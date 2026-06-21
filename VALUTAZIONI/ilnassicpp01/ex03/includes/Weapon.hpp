/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 02:21:46 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/31 02:28:58 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPON_HPP
# define WEAPON_HPP

#include <string>

class Weapon
{
    private:
        std::string _type;

    public:
        Weapon(std::string type);
        ~Weapon();
        /*Regola d'oro del C++Ogni volta che scrivi una funzione membro (un metodo dentro una classe) che si limita a leggere un dato (i cosiddetti getter) e non modifica le variabili interne della classe, devi sempre mettere il const alla fine.*/
        const std::string& getType() const ;
        void    setType(std::string type);
};

#endif
