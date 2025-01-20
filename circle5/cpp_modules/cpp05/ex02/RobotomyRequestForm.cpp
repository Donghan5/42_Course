/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RobotomyRequestForm.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:55:26 by donghank          #+#    #+#             */
/*   Updated: 2024/12/28 14:20:33 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RobotomyRequestForm.hpp"

/*
	Default constructor
	?? target Init이 뒤에 오는 이유
*/
RobotomyRequestForm::RobotomyRequestForm(): AForm("RobotomyRequestForm", 72, 45), _target("Default") {
	std::cout << "Default RobotomyRequestForm has been called" << std::endl;
}

/*
	Type(name) constructor
	@param
		target: target name
*/
RobotomyRequestForm::RobotomyRequestForm(const std::string &target): AForm("RobotomyRequestForm", 72, 45), _target(target) {
	std::cout << "RobotomyRequestForm constructor " << this->_target << " has been called" << std::endl;
}

/*
	Copy constructor
	@param
		rhs: copy obj
*/
RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm &rhs): AForm(rhs), _target(rhs._target) {
	std::cout << "Copy constructor " << this->_target << " has been called" << std::endl;
}

/*
	Destructor
*/
RobotomyRequestForm::~RobotomyRequestForm() {
	std::cout << "Destructor of RobotomyRequestForm " << this->_target << " has been called" << std::endl;
}

/*
	Copy assignement
	@param
		rhs: obj
	@return
		this = self-pointer
*/
RobotomyRequestForm	&RobotomyRequestForm::operator=(const RobotomyRequestForm &rhs) {
	if (this != &rhs) {
		AForm::operator=(rhs);
		this->_target = rhs._target;
	}
	return (*this);
}

/*
	getter part
	@return
		this->_target: self-pointer of the class
*/
const std::string	&RobotomyRequestForm::getTarget(void) const {
	return this->_target;
}

void	RobotomyRequestForm::execute(const Bureaucrat &executor) const {
	this->checkExecutability(executor);

	std::cout << "Make some drilling noises..." << std::endl;
	if (rand() % 2)
		std::cout << "GRRRR.... Robotomize of " << this->_target << " has been successfully done" << std::endl;
	else
		std::cout << "UMMMM... Robotomize of " << this->_target << " has been failed..." << std::endl;
}
