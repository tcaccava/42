/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:37:48 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:14:00 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MateriaSource.hpp"
#include "Ice.hpp"
#include "Cure.hpp"

MateriaSource::MateriaSource()
{
    for(int i=0; i < 4; i++)
        learnedMaterias[i] = NULL;
}

MateriaSource::MateriaSource(const MateriaSource &obj)
{
    for (int i = 0; i < 4; i++)
    {
        if (obj.learnedMaterias[i])
            learnedMaterias[i] = obj.learnedMaterias[i]->clone();
        else
            learnedMaterias[i] = NULL;
    }
}

MateriaSource& MateriaSource::operator=(const MateriaSource &obj)
{

    if (this != &obj)
    {
        for (int i = 0; i < 4; i++)
        {
            if(learnedMaterias[i])
                delete learnedMaterias[i];
            if (obj.learnedMaterias[i])
                learnedMaterias[i] = obj.learnedMaterias[i]->clone();
            else
                learnedMaterias[i] = NULL;
        }
    }
    return *this;
}

MateriaSource::~MateriaSource()
{
    for(int i = 0;i < 4; i++)
        if(learnedMaterias[i])
            delete learnedMaterias[i];
}

void MateriaSource::learnMateria(AMateria* m)
{
    int i = -1;
    while (++i < 4 && learnedMaterias[i])
    ;
        if (i < 4)
            learnedMaterias[i] = m->clone();  
        else
        std::cout << "Cannot learn because inventory is full" << std::endl;
}

AMateria* MateriaSource::createMateria(std::string const & type)
{
        for (int i = 0; i < 4; i++) 
        {
            if (learnedMaterias[i] && learnedMaterias[i]->getType() == type)
                return learnedMaterias[i]->clone();
        }
    std::cout << "Type not found" << std::endl;
    return 0;
}