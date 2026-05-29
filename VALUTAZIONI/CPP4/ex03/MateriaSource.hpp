/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rauer <rauer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:20:19 by rauer             #+#    #+#             */
/*   Updated: 2025/03/14 17:20:29 by rauer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIASOURCE_HPP
#define MATERIASOURCE_HPP

#include <string>
#include <iostream>
#include "AMateria.hpp"
#include "IMateriaSource.hpp"

class MateriaSource : public IMateriaSource
{
private:
	AMateria *_materias[4];

public:
	MateriaSource();
	MateriaSource(MateriaSource const &src);
	~MateriaSource();

	MateriaSource &operator=(MateriaSource const &rhs);
	void learnMateria(AMateria *materia);
	AMateria *createMateria(std::string const &type);
};

#endif /* *************************************************** MATERIASOURCE_H */
