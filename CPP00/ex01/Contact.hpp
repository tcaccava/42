#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <iostream>
#include <iomanip>

class Contact {
public:
    void setContact();
    void printDetails() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getNickname() const;

private:
    std::string first_name;
    std::string last_name;
    std::string nickname;
    std::string phone_number;
    std::string darkest_secret;
};

#endif