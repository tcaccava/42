/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easyfind.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcalabre <pcalabre@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:32:59 by pcalabre          #+#    #+#             */
/*   Updated: 2025/03/04 18:33:03 by pcalabre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EASYFIND_HPP
#define EASYFIND_HPP

#include <algorithm>
#include <iostream>

template <typename T> typename T::iterator easyfind(T &t, int n) {
    return std::find(t.begin(), t.end(), n);
}

#endif
