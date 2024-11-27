/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhoneBook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:02:19 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 17:12:46 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PhoneBook.hpp"

/*
	print the strings and alignment right
*/
void	printName(std::string str) {
	if (str.length() > 10)
		std::cout << str.substr(0, 9) << ".";
	else
	{
		for (unsigned int j = 0; j < 10 - str.length() ; j++)
			std::cout << " ";
		std::cout << str;
	}
}

/*
	to calculate the size
*/
int	PhoneBook::getSize() const {
	return (this->size);
}

PhoneBook::PhoneBook() {
	this->index = 0;
	this->size = 0;
	std::cout << "\033[1;31mWelcome\033[0m" << std::endl;
}

PhoneBook::~PhoneBook() {
	std::cout << "\033[1;31mSee ya!\033[0m" << std::endl;
}

/*
	add the contact data[0] --> data[5]
	each data is represent firstname lastname nickname secret and phone number
*/
void	PhoneBook::addContact(std::string data[5]) {
	int	index;

	index = this->index;
	this->contacts[index].setFirstname(data[0]);
	this->contacts[index].setLastname(data[1]);
	this->contacts[index].setNickname(data[2]);
	this->contacts[index].setNumber(data[3]);
	this->contacts[index].setSecret(data[4]);
	this->index = (index + 1) % 8; // to sure the range 0 to 7
	if (this->size < 8)
		this->size++;
}

/*
	to disply the contact
	After input the SEARCH keyword
*/
void	PhoneBook::displayContact(int i) const {
	std::cout << "This is for the specified contact" << std::endl;

	std::cout << "First Name: " << this->contacts[i].getFirstname() << std::endl;
	std::cout << "Last Name: " << this->contacts[i].getLastname() << std::endl;
	std::cout << "Nick Name: " << this->contacts[i].getNickname() << std::endl;
	std::cout << "Phone Number: " << this->contacts[i].getNumber() << std::endl;
	std::cout << "Darkest Secret: " << this->contacts[i].getSecret() << std::endl;
}

/*
	to display the phonebook
*/
void	PhoneBook::displayPhonebook() const {
	int	i = 0;

	// print the format (to print the information)
	std::cout << "Index | First Name | Last Name | Nick Name" << std::endl;

	while (i < this->size) {
		std::cout << i << " | ";
		printName(this->contacts[i].getFirstname());
		std::cout << " | ";
		printName(this->contacts[i].getLastname());
		std::cout << " | ";
		printName(this->contacts[i].getNickname());
		std::cout << std::endl;
		i++;
	}
}
