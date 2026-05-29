/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyriarte <jyriarte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:19:23 by jyriarte          #+#    #+#             */
/*   Updated: 2025/06/05 16:19:27 by jyriarte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cfloat>
#include <climits>
#include <cstdlib>
#include <iostream>

enum Type
{
    CHAR = 1 << 1,
    INT = 1 << 2,
    FLOAT = 1 << 3,
    DOUBLE = 1 << 4,
};

class ScalarConverter
{
  public:
    static void convert(const std::string &param);

  private:
    ScalarConverter(void);
};
