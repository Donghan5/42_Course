/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 12:55:26 by donghank          #+#    #+#             */
/*   Updated: 2024/11/03 19:56:10 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"
#include "PhoneBook.hpp"

/*
	to verify input number value (phone number)
	@param : number --> phone number
	@return : there is 1 if not 0
*/
static int	isValidNumber(std::string number) {
	int	i = 0;
	while (number[i]) {
		if (number[i] == ' ') {
			i++;
			continue ;
		}
		if (number[i] < '0' || number[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/*
	to treat getline errors
*/
bool	getInput(std::istream& inputStream, std::string& output) {
	if (std::getline(inputStream, output)) {
		return (true); // Success input
	}
	else {
		if (inputStream.eof()) {
			std::cerr << "EOF detected" << std::endl;
			exit(1);
		}
		std::cerr << "Input error" << std::endl; // print an error msg
		std::cin.clear(); // clean the input buffer
		return (false);
	}
}

/*
	new notion : do-while loop is efficient in the way user input
*/
int	main(void) {
	PhoneBook	phonebook;
	std::string	data[5];
	std::string	input;
	int			index;

	while (1) {
		std::cout << "\033[1;31mPlease enter ADD, SEARCH or EXIT in CAPITAL\033[0m" << std::endl;
		std::cout << "Phone Book > ";
		getInput(std::cin, input);
		// ADD mode
		if (input == "ADD") {
			std::cout << "Enter Contact Informations" << std::endl;
			std::cout << "First Name: ";
			getInput(std::cin, data[0]);
			std::cout << "Last Name: ";
			getInput(std::cin, data[1]);
			std::cout << "Nick Name: ";
			getInput(std::cin, data[2]);
			do {
				std::cout << "Phone Number: ";
				getInput(std::cin, data[3]);
				if (isValidNumber(data[3]))
					break ;
				std::cout << "Please input the valid number" << std::endl;
			} while (!isValidNumber(data[3]));
			std::cout << "Darkest Secret: ";
			getInput(std::cin, data[4]);
			phonebook.addContact(data);
			std::cout << "\033[1;31mADD is successfully done\033[0m" << std::endl;
		}
		// SEARCH mode
		else if (input == "SEARCH") {
			phonebook.displayPhonebook();
			std::cout << "Insert index: ";
			std::cin >> index;
			if (std::cin.fail() || index < 0 || index >= phonebook.getSize()) {
				std::cout << "Invalid input" << std::endl;
				std::cin.clear();
				getInput(std::cin, input);
				continue ;
			}
			phonebook.displayContact(index);
			std::cin.clear();
			getInput(std::cin, input);
			continue ;
		}
		// EXIT mode
		else if (input == "EXIT") {
			std::cout << "Exit the phonebook" << std::endl;
			break ;
		}
		else
			std::cout << "Invalid command" << std::endl;
	}
	return (0);
}
