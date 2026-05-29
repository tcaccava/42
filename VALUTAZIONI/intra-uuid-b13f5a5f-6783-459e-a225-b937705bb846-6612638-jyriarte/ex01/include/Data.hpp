/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyriarte <jyriarte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:35:16 by jyriarte          #+#    #+#             */
/*   Updated: 2025/06/10 11:59:50 by jyriarte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class Data
{
  public:
    Data(void);
    Data(const std::string &content);
    Data(const Data &other);
    ~Data(void);

    Data &operator=(const Data &other);

    const std::string &getContent(void);
    void setContent(const std::string &content);

  private:
    std::string _content;
};
