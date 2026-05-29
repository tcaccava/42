/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:41:29 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/05 10:41:30 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HARL_HPP
#define HARL_HPP

#include <iostream>

class Harl {

private:
	typedef void (Harl::*HarlFunction)();
	static const std::string levels[];
	static const HarlFunction functions[];

	void		debug( void );
	void		info( void );
	void		warning( void );
	void		error( void );

public:
	Harl();

	void	complain( std::string level );

	~Harl();
};

#endif
