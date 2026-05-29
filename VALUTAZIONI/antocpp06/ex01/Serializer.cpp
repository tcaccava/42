/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serializer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:04:05 by apintaur          #+#    #+#             */
/*   Updated: 2025/09/04 09:36:43 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serializer.hpp"
#include <cstdlib>

uintptr_t Serializer::serialize(Data* ptr) {

	if (!ptr)
		return (0x0);
	return (reinterpret_cast<uintptr_t>(ptr));
}

Data*	Serializer::deserialize(uintptr_t raw) {

	if (raw != 0x0)
		return (reinterpret_cast<Data*>(raw));
	return (NULL);
}
