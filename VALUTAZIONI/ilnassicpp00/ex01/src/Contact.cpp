/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:53:13 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 12:05:18 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"

/*Costruttore di default del contatto.
poi, imposta nome, cognome, numero e segreto (setter).
Con string vengono restituiti (getter).*/
Contact::Contact() {}

void Contact::setFirstName(std::string name)       { _firstName = name; }
void Contact::setLastName(std::string name)        { _lastName = name; }
void Contact::setNickname(std::string name)        { _nickname = name; }
void Contact::setPhoneNumber(std::string number)   { _phoneNumber = number;}
void Contact::setDarkestSecret(std::string secret) { _darkestSecret = secret; }

std::string Contact::getFirstName() const        { return _firstName; }
std::string Contact::getLastName() const         { return _lastName; }
std::string Contact::getNickname() const         { return _nickname; }
std::string Contact::getPhoneNumber() const      { return _phoneNumber; }
std::string Contact::getDarkestSecret() const  { return _darkestSecret; }
