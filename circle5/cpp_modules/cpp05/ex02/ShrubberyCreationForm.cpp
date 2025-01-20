/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShrubberyCreationForm.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:52:11 by donghank          #+#    #+#             */
/*   Updated: 2024/12/28 14:32:48 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShrubberyCreationForm.hpp"

/*
	Default constructor
*/
ShrubberyCreationForm::ShrubberyCreationForm(): AForm("ShruberryCreationForm", 145, 137), _target("Default") {
	std::cout << "Default ShrubberyCreationForm constructor has been called" << std::endl;
}

/*
	Type(name) constructor
	@param
		target: name of the target
*/
ShrubberyCreationForm::ShrubberyCreationForm(const std::string &target): AForm("ShruberryCreationForm", 145, 137), _target(target) {
	std::cout << "ShrubberyCreationForm constructor " << this->_target << " has been called" << std::endl;
}

/*
	copy constructor
	@param
		rhs: copy obj
*/
ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm &rhs): AForm(rhs), _target(rhs._target) {
	std::cout << "ShrubberyCreationForm copy constructor " << this->_target <<  " has been called" << std::endl;
}

/*
	Destructor
*/
ShrubberyCreationForm::~ShrubberyCreationForm() {
	std::cout << "ShrubberyCreationForm destructor " << this->_target << " has been called" << std::endl;
}

/*
	Copy assignement
	@param
		rhs: copy obj
	@return
		this: self-pointer
*/
ShrubberyCreationForm	&ShrubberyCreationForm::operator=(const ShrubberyCreationForm &rhs) {
	if (this != &rhs) {
		AForm::operator=(rhs);
		this->_target = rhs._target;
	}
	return (*this);
}
/*
	getter
	@return
		this->_target : target of class
*/
const std::string	&ShrubberyCreationForm::getTarget(void) const {
	return this->_target;
}

void	ShrubberyCreationForm::execute(const Bureaucrat &executor) const {
	std::ofstream	file;
	std::string		name;

	this->checkExecutability(executor);
	name = _target + "_shrubbery";
	file.open(name.c_str(), std::ofstream::out);

	file << "      # #### ####               " << std::endl;
	file << "     ### \\/#|### |/####        " << std::endl;
	file << "    ##\\/#/ \\||/##/_/##/_#     " << std::endl;
	file << "  ###  \\/###|/ \\/ # #####     " << std::endl;
	file << " ##_\\_#\\_\\## | #/###_/_####  " << std::endl;
	file << "## #### # \\ #| /  #### ##/##   " << std::endl;
	file << " __#_--###`  |{,###---###-~     " << std::endl;
	file << "          \\ }{                 " << std::endl;
	file << "           }}{                  " << std::endl;
	file << "           }}{                  " << std::endl;
	file << "      kdh  {{}                  " << std::endl;
	file << "     , -=-~{ .-^- _             " << std::endl;
	file << "           `}                   " << std::endl;
	file << "            {                   " << std::endl;

	file.close();
}

