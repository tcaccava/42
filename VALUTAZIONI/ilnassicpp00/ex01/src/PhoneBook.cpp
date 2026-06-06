/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhoneBook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:53:24 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 14:46:20 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include "PhoneBook.hpp"

/*Inizializza la rubrica vuota. (initialization list)
_count tiene traccia del numero totale di contatti aggiunti.*/
PhoneBook::PhoneBook() : _count(0) {}

/*Se la stringa supera 10 caratteri,
la tronca ai primi 9 e aggiunge un punto finale.
Serve per rispettare il formato richiesto dalla SEARCH.*/
static std::string truncate(std::string str)
{
    if (str.length() > 10)
        return str.substr(0, 9) + ".";
    return str;
}


/*Chiede all'utente tutti i dati del contatto.
Se uno dei campi è vuoto il contatto non viene salvato.
I contatti vengono salvati in modo circolare:
quando si supera il limite di 8 contatti,
il più vecchio viene sovrascritto
Uso il modulo (%) per ottenere sempre un indice
compreso tra 0 e 7.*/
void PhoneBook::addContact()
{
    Contact newContact;
    std::string input;
    int index;

    index = _count % 8;

    std::cout << "First name: ";
    std::getline(std::cin, input);
    if (input.empty()) 
    {
        std::cout << "Is empty!\n"; 
        return; 
    }
    newContact.setFirstName(input);

    std::cout << "Last name: ";
    std::getline(std::cin, input);
    if (input.empty())
    {
        std::cout << "Is empty!\n";
        return;
    }
    newContact.setLastName(input);

    std::cout << "Nickname: ";
    std::getline(std::cin, input);
    if (input.empty())
    {
        std::cout << "Is empty!\n";
        return;
    }
    newContact.setNickname(input);

    std::cout << "Phone number: ";
    std::getline(std::cin, input);
    if (input.empty())
    {
        std::cout << "Is empty!\n";
        return;
    }
    newContact.setPhoneNumber(input);

    std::cout << "Darkest secret: ";
    std::getline(std::cin, input);
    if (input.empty())
    {
        std::cout << "Is empty!\n";
        return;
    }
    newContact.setDarkestSecret(input);

    _contacts[index] = newContact;
    _count++;
}

/*Mostra la lista dei contatti salvati in formato tabellare.
Successivamente chiede un indice all'utente e,
se valido, stampa tutte le informazioni del contatto.
Le stringhe troppo lunghe vengono troncate
tramite la funzione truncate().
Verifica che l'utente abbia inserito
un valore numerico valido.*/
void PhoneBook::searchContact() const
{
    int total;
    std::string input;
    int index;

    if (_count < 8)
        total = _count;
    else
        total = 8;

    if (total == 0)
    {
        std::cout << "Phone book is empty!\n";
        return;
    }

    std::cout << std::setw(10) << "Index"   << "|"
              << std::setw(10) << "First"   << "|"
              << std::setw(10) << "Last"    << "|"
              << std::setw(10) << "Nickname" << "|"
              << std::endl;

    for (int i = 0; i < total; i++)
    {
        std::cout << std::setw(10) << std::right << i                                     << "|"
                  << std::setw(10) << std::right << truncate(_contacts[i].getFirstName()) << "|"
                  << std::setw(10) << std::right << truncate(_contacts[i].getLastName())  << "|"
                  << std::setw(10) << std::right << truncate(_contacts[i].getNickname())  << "|"
                  << std::endl;
    }

    std::cout << "Enter index: ";
    std::getline(std::cin, input);

    bool valid = !input.empty();

    for (size_t i = 0; i < input.length() && valid; i++)
    {
        if (!std::isdigit(input[i]))
            valid = false;
    }

    if (!valid)
    {
        std::cout << "Invalid index!" << std::endl;
        return;
    }

    index = std::atoi(input.c_str());

    if (index < 0 || index >= total)
    {
        std::cout << "Index out of range!" << std::endl;
        return;
    }

    std::cout << "First name:     " << _contacts[index].getFirstName()    << std::endl;
    std::cout << "Last name:      " << _contacts[index].getLastName()     << std::endl;
    std::cout << "Nickname:       " << _contacts[index].getNickname()     << std::endl;
    std::cout << "Phone number:   " << _contacts[index].getPhoneNumber()  << std::endl;
    std::cout << "Darkest secret: " << _contacts[index].getDarkestSecret()<< std::endl;
}
