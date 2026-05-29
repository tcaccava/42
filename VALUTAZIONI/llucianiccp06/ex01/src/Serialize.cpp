/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serialize.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:24:17 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/27 16:24:20 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serialize.hpp"

Serialize::Serialize()
{

}

Serialize::Serialize(const Serialize& other)
{
	(void)other;
}

Serialize& Serialize::operator=(const Serialize& other)
{
	if(this != &other)
	{

	}
	return *this;
}

Serialize::~Serialize()
{

}

uintptr_t Serialize::serialize(Data* ptr)
{
	return reinterpret_cast<uintptr_t>(ptr);
}

Data* Serialize::deserialize(uintptr_t raw)
{
	return reinterpret_cast<Data*>(raw);
}
