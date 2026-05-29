/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <lluciani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 19:10:04 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/28 19:10:04 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iter.hpp"

int main()
{
    std::string stringArray[4] = {"hi", "hello", "World", "bye"};
    iter(stringArray, 4, function<std::string>);

    int intArray[6] = {2, 4, 6, 8, 10, 12};
    iter(intArray, 6, function<int>);

}