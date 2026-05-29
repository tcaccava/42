#ifndef CONTACT_H_HPP
# define CONTACT_H_HPP

# include <iostream>

class Contact
{
	private:
		std::string firstName;
		std::string lastName;
		std::string nickname;
		std::string phoneNumber;
		std::string darkestSecret;

	public:
		Contact();
		Contact ( const Contact &contact );
		~Contact ();
		Contact & operator=(const Contact &contact);

		void setFirstName(std::string value);
		void setLastName(std::string value);
		void setNickname(std::string value);
		void setPhoneNumber(std::string value);
		void setDarkestSecret(std::string value);

		void update(
			std::string firstName, std::string lastName, std::string nickname, std::string phoneNumber, std::string darkestSecret
		);

		std::string getFirstName(void) const;
		std::string getLastName(void) const;
		std::string getNickname(void) const;
		std::string getPhoneNumber(void) const;
		std::string getDarkestSecret(void) const;

		void printInfo(void) const;
};

#endif
