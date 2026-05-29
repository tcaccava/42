/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:19:13 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 18:12:13 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include "AMateria.hpp"
#include "MateriaSource.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MateriaSource::MateriaSource()
	: _materias()
{
	std::cout << "MateriaSource default constructor called." << std::endl;
	std::cout << "\tMateria 0: " << this->_materias[0] << std::endl;
	std::cout << "\tMateria 1: " << this->_materias[1] << std::endl;
	std::cout << "\tMateria 2: " << this->_materias[2] << std::endl;
	std::cout << "\tMateria 3: " << this->_materias[3] << std::endl;
}

MateriaSource::MateriaSource(const MateriaSource &src)
	: _materias()
{
	std::cout << "MateriaSource copy constructor called." << std::endl;
	std::cout << "\tMateria 0: " << this->_materias[0] << std::endl;
	std::cout << "\tMateria 1: " << this->_materias[1] << std::endl;
	std::cout << "\tMateria 2: " << this->_materias[2] << std::endl;
	std::cout << "\tMateria 3: " << this->_materias[3] << std::endl;
	int i = 0;
	while (i < 4)
	{
		this->_materias[i] = 0;
		if (src._materias[i])
			this->_materias[i] = src._materias[i]->clone();
		i++;
	}
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

MateriaSource::~MateriaSource()
{
	std::cout << "MateriaSource destructor called." << std::endl;
	int i = 0;
	while (i < 4)
	{
		if (this->_materias[i])
			delete this->_materias[i];
		i++;
	}
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

MateriaSource &MateriaSource::operator=(MateriaSource const &rhs)
{
	if (this != &rhs)
	{
		int i = 0;
		while (i < 4)
		{
			if (this->_materias[i])
				delete this->_materias[i];
			if (rhs._materias[i])
				this->_materias[i] = rhs._materias[i]->clone();
			else
				this->_materias[i] = 0;
			i++;
		}
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void MateriaSource::learnMateria(AMateria *materia)
{
	int i = 0;
	while (i < 4)
	{
		if (!this->_materias[i])
		{
			this->_materias[i] = materia;
			return;
		}
		i++;
	}
}

AMateria *MateriaSource::createMateria(std::string const &type)
{
	int i = 0;
	while (i < 4)
	{
		if (this->_materias[i] && this->_materias[i]->getType() == type)
			return this->_materias[i]->clone();
		i++;
	}
	return 0;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */
