/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:50 by donghank          #+#    #+#             */
/*   Updated: 2024/11/23 15:51:05 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"

/*
	constructor
*/
Harl::Harl() {
	// std::cout << "Constructor of Harl has been called" << std::endl;
}

/*
	destructor
*/
Harl::~Harl() {
	// std::cout << "Destructor of Harl has been called" << std::endl;
}

void	Harl::debug(void) {
	std::cout <<  "I love having extra bacon for my 7XL-double-cheese-triple-pickle-specialketchup burger. I really do!" << std::endl;
}

void	Harl::info(void) {
	std:: cout << "I cannot believe adding extra bacon costs more money. You did not put enough bacon in my burger! If you did, I would not be asking for more!" << std::endl;
}

void	Harl::warning(void) {
	std::cout << "I think I deserve to have some extra bacon for free. I have been coming for years whereas you started working here since last month." << std::endl;
}

void	Harl::error(void) {
	std::cout << "This is unacceptable! I want to speak to the manager now." << std::endl;
}

void	Harl::complain(std::string level) {
	std::string	levels[4] = {"DEBUG", "INFO", "WARNING", "ERROR"}; //init --> using list
	void	(Harl::*f[4]) (void) = {&Harl::debug, &Harl::info, &Harl::warning, &Harl::error}; // pointer of the function

	for (int i = 0; i < 4; i++) {
		if (levels[i] == level)
			(this->*f[i])(); // to call the function
	}
}
