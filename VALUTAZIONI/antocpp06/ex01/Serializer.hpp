/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serializer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:04:07 by apintaur          #+#    #+#             */
/*   Updated: 2025/09/04 09:38:52 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include "Data.hpp"
#include <stdint.h>

class Serializer {

  private:
	Serializer();
	Serializer(const Serializer& s);
	Serializer& operator=(const Serializer& s);
	~Serializer();

  public:
	static uintptr_t serialize(Data* ptr);
	static Data*	 deserialize(uintptr_t raw);
};

#endif
