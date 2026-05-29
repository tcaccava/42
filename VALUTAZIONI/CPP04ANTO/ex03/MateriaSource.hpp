/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:15:02 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 16:30:03 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIASOURCE_HPP
#define MATERIASOURCE_HPP

#include "IMateriaSource.hpp"
#include "AMateria.hpp"

#define MAX_MATERIAS 4

class MateriaSource : public IMateriaSource {

	private:
		AMateria*	materias[MAX_MATERIAS];


	public:
		MateriaSource();
		MateriaSource( const MateriaSource& materiaSource);

		MateriaSource&	operator=( const MateriaSource& materiaSource );

		virtual void learnMateria( AMateria* );
		virtual AMateria* createMateria( std::string const & type );

		~MateriaSource();

};

#endif
