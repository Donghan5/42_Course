/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Intern.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 11:20:39 by donghank          #+#    #+#             */
/*   Updated: 2024/12/28 14:58:47 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Intern.hpp"

/*
	Default constructor
*/
Intern::Intern() {
	std::cout << "Default constructor of Intern has been called" << std::endl;
}

/*
	Copy constructor
	@param
		rhs: copy obj
*/
Intern::Intern(const Intern &rhs) {
	*this = rhs;
	std::cout << "Copy constructor of Intern has been called" << std::endl;
}

/*
	Copy assignement
*/
Intern	&Intern::operator=(const Intern &rhs) {
	(void) rhs;
	return *this;
}

/*
	Destructor
*/
Intern::~Intern() {
	std::cout << "Destructor of Intern has been called" << std::endl;
}

AForm	*Intern::makeForm(std::string name, std::string target) {
	std::string	nameForm[] = {
		"robotomy request",
		"shurbbery creation",
		"presidential pardon",
	};
	AForm	*forms[] = {
		new RobotomyRequestForm(target),
		new ShrubberyCreationForm(target),
		new PresidentialPardonForm(target)
	};
	for (int i = 0; i < 3; i++)
	{
		if (name == nameForm[i])
		{
			std::cout << "Intern creates " << name << std::endl;
			// just remain the obj which want to return
			for (int j = 0; j < 3; j++) {
				if (j != i)
					delete forms[j];
			}
			return forms[i];
		}
	}
	// case doesn't match, delete all instances
	for (int i = 0; i < 3; i++) {
		delete forms[i];
	}
	std::cerr << "Intern cannot create form" << std::endl;
	return NULL;
}
