#ifndef PHONEBOOK_H_HPP
# define PHONEBOOK_H_HPP

# include <iostream>
# include <iomanip>
# include "Contact.hpp"

# define CONTACTS_NUM 8
# define COLUMN_LEN 10

class PhoneBook
{
    private:
        Contact contacts[CONTACTS_NUM];
        size_t  contacts_saved;
        size_t  oldest_index;

        void    printHeader() const;
        void    printValue(std::string value) const;

    public:
        PhoneBook();
		PhoneBook ( const PhoneBook &contact );
		~PhoneBook ();
		PhoneBook & operator=(const PhoneBook &contact);

        void    addContact();
        void    search() const;
};

#endif
