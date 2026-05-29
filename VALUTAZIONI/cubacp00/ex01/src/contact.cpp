#include "../inc/Contact.hpp"

Contact::Contact() {
    this->firstName = "";
    this->lastName = "";
    this->nickname = "";
    this->phoneNumber = "";
    this->darkestSecret = "";
}

void Contact::setFirstName(std::string value) {
    this->firstName = value;
}
void Contact::setLastName(std::string value) {
    this->lastName = value;
}
void Contact::setNickname(std::string value) {
    this->nickname = value;
}
void Contact::setPhoneNumber(std::string value) {
    this->phoneNumber = value;
}
void Contact::setDarkestSecret(std::string value) {
    this->darkestSecret = value;
}

std::string Contact::getFirstName(void) const {
    return this->firstName;
}
std::string Contact::getLastName(void) const {
    return this->lastName;
}
std::string Contact::getNickname(void) const {
    return this->nickname;
}
std::string Contact::getPhoneNumber(void) const {
    return this->phoneNumber;
}
std::string Contact::getDarkestSecret(void) const {
    return this->darkestSecret;
}

void Contact::update(std::string firstName, std::string lastName, std::string nickname, std::string phoneNumber, std::string darkestSecret) {
    this->setFirstName(firstName);
    this->setLastName(lastName);
    this->setNickname(nickname);
    this->setPhoneNumber(phoneNumber);
    this->setDarkestSecret(darkestSecret);
}

void Contact::printInfo() const
{
    std::cout << "First Name: " << this->getFirstName() << std::endl;
    std::cout << "Last Name: " << this->getLastName() << std::endl;
    std::cout << "Nickname: " << this->getNickname() << std::endl;
    std::cout << "Phone Number: " << this->getPhoneNumber() << std::endl;
    std::cout << "Darkest Secret: " << this->getDarkestSecret() << std::endl;
}

Contact::Contact ( const Contact &contact )
{
    this->setDarkestSecret(contact.getDarkestSecret());
    this->setFirstName(contact.getFirstName());
    this->setLastName(contact.getLastName());
    this->setNickname(contact.getNickname());
    this->setPhoneNumber(contact.getPhoneNumber());
}

Contact::~Contact () {

}

Contact & Contact::operator=(const Contact &contact) {
    this->setDarkestSecret(contact.getDarkestSecret());
    this->setFirstName(contact.getFirstName());
    this->setLastName(contact.getLastName());
    this->setNickname(contact.getNickname());
    this->setPhoneNumber(contact.getPhoneNumber());

    return *this;
}