#ifndef PHONEBOOK_HPP
#define PHONEBOOK_HPP

#include <iostream>
#include <iomanip>
#include "Contact.hpp"

class PhoneBook {
    public: 
        PhoneBook();
        void addContact();
        void searchContact();

    private: 
        Contact Contacts[8];
        int index; 
        int total_contacts;
};

#endif