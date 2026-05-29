/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serialize.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:23:17 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/27 16:23:25 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <iostream>
#include "Data.hpp"
#include <stdint.h>

class Serialize
{
	private:
			Serialize();
			Serialize(const Serialize& other);
			Serialize& operator=(const Serialize& other);
			~Serialize();
	public:
			static uintptr_t serialize(Data* ptr);
			static Data* deserialize(uintptr_t raw);

};

#endif
