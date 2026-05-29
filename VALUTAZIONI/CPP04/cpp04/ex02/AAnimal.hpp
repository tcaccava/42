/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAnimal.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:07:15 by mla-mona          #+#    #+#             */
/*   Updated: 2025/04/05 20:26:45 by mla-mona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AANIMAL_HPP
#define AANIMAL_HPP

#include <string>

class AAnimal {
protected:
    std::string type;
public:
    AAnimal();  // Costruttore protetto
    virtual ~AAnimal();  // Distruttore virtuale
    virtual void makeSound() const = 0;  // Funzione virtuale pura
    std::string getType() const;
};

#endif

