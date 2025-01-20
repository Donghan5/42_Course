/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PresidentialPardonForm.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:59:48 by donghank          #+#    #+#             */
/*   Updated: 2024/12/27 16:08:06 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PresidentialPardonForm.hpp"

/*
	Default constructor
*/
PresidentialPardonForm::PresidentialPardonForm(): AForm("PresidentialPardonForm", 25, 5), _target("Default") {
	std::cout << "Default constructor PresidentialPardonForm has been called" << std::endl;
}

/*
	Type (name) constructor
	@param
		target: the name of the target
*/
PresidentialPardonForm::PresidentialPardonForm(const std::string &target): AForm("PresidentialPardonForm", 25, 5), _target(target) {
	std::cout << "Constructor PresidentialPardonForm " << this->_target << " has been called" << std::endl;
}

/*
	Copy constructor
	@param
		rhs: copy obj
*/
PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm &rhs): AForm(rhs), _target(rhs._target) {
	std::cout << "Copy constructor PresidentialPardonForm" << this->_target << " has been called" << std::endl;
}

/*
	Destructor
*/
PresidentialPardonForm::~PresidentialPardonForm() {
	std::cout << "Destructor PresidentialPardonForm " << this->_target << " has been called" << std::endl;
}

/*
	Copy assignement
	@param
		rhs: obj
	@return
		this = self-pointer
*/
PresidentialPardonForm	&PresidentialPardonForm::operator=(const PresidentialPardonForm &rhs) {
	if (this != &rhs) {
		AForm::operator=(rhs);
		this->_target = rhs._target;
	}
	return (*this);
}

/*
	getter function
	@return
		this->_target
*/
const std::string	&PresidentialPardonForm::getTarget(void) const {
	return this->_target;
}

/*
	required function
	@param
		executor
*/
void	PresidentialPardonForm::execute(const Bureaucrat &executor) const {
	this->checkExecutability(executor);
	std::cout << this->_target << " has been pardoned by Zaphod Beeblebrox." << std::endl;
}
