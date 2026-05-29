/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyriarte <jyriarte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:00:35 by jyriarte          #+#    #+#             */
/*   Updated: 2025/06/10 12:22:55 by jyriarte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Data.hpp"
#include "Serializer.hpp"

int main(void)
{
    Data testData("ORIGINAL CONTENT");
    uintptr_t container = Serializer::serialize(&testData);
    Data *data = Serializer::deserialize(container);

    std::cout << data->getContent() << std::endl;
    data->setContent("NEW CONTENT");
    std::cout << testData.getContent() << std::endl;

    std::cout << std::endl;
    std::cout << &testData << std::endl;
    std::cout << data << std::endl;
    return (0);
}
