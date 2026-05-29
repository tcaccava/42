/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:53:48 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/11 21:33:54 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>

static const int MAX_IDEAS = 100;

class Brain {

	private:
		std::string ideas[MAX_IDEAS];
		int			head;
		int			tail;
		int			size;

	public:
		Brain();
		Brain( const std::string& idea );
		Brain( const Brain& brain );

		Brain&	operator=( const Brain& brain );

		const std::string&		getIdea( const int& idx ) const;
		void					setIdea( const std::string& idea );
		bool					isFull( void ) const { return (size == MAX_IDEAS); }
		bool					isEmpty( void ) const { return (size == 0); }

		~Brain();
};

#endif
