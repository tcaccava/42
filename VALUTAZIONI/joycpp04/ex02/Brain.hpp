/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:35:40 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/31 16:17:13 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>

class Brain 
{
    private:
        std::string ideas[100];
    public:
        Brain();
        Brain(const Brain &instance);
        Brain& operator=(const Brain &instance);
        ~Brain();
};

#endif