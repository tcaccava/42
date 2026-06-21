/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:52:24 by ilnassi           #+#    #+#             */
/*   Updated: 2026/06/05 19:44:10 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLACE_HPP
# define REPLACE_HPP

# include <string>
# include <fstream>
# include <iostream>

std::string replaceLine(
	std::string line, 
	const std::string& s1, 
	const std::string& s2);

#endif
