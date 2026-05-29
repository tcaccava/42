/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyriarte <jyriarte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:40:43 by jyriarte          #+#    #+#             */
/*   Updated: 2025/06/10 11:43:12 by jyriarte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Data.hpp"

Data::Data(void)
{
}

Data::Data(const std::string &content) : _content(content)
{
}

Data::Data(const Data &other)
{
    _content = other._content;
}

Data::~Data(void)
{
}

Data &Data::operator=(const Data &other)
{
    _content = other._content;
    return (*this);
}

const std::string &Data::getContent(void)
{
    return (_content);
}

void Data::setContent(const std::string &content)
{
    _content = content;
}
