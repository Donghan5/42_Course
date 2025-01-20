/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Form.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:51:42 by donghank          #+#    #+#             */
/*   Updated: 2024/12/28 14:35:20 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Form.hpp"

/*
	Exception function
*/
const char	*Form::GradeTooHighException::what() const throw() {
	return "Grade is too high";
}

/*
	Exception function
*/
const char	*Form::GradeTooLowException::what() const throw() {
	return "Grade is too low";
}

/*
	Default constructor
*/
Form::Form(): _name("Default"), _gradeToSign(1), _gradeToExec(1), _signed(false) {
	std::cout << "Default constructor has been called" << std::endl;
}

/*
	Type(name) constructor
	@param
		name: name of the class
		gradeToSign: required grade to sign
		gradeToExec: required grad to execution
*/
Form::Form(const std::string &name, int gradeToSign, int gradeToExec): _name(name), _gradeToSign(gradeToSign), _gradeToExec(gradeToExec) {
	if (gradeToSign < 1 || gradeToExec < 1)
		throw GradeTooHighException();
	else if (gradeToSign > 150 || gradeToExec > 150)
		throw GradeTooLowException();
	this->_signed = false;
	std::cout << "Constructor " << this->_name << " has been called" << std::endl;
}

/*
	Copy constructor
	@param
		rightSide: copy obj
*/
Form::Form(const Form &rightSide): _name(rightSide._name), _gradeToSign(rightSide._gradeToSign), _gradeToExec(rightSide._gradeToExec), _signed(rightSide._signed) {
	std::cout << "Copy constructor " << this->_name << " has been called" << std::endl;
}

/*
	Copy assingment
	@param
		rightSide: copy obj
*/
Form	&Form::operator=(const Form &rightSide) {
	if (this != &rightSide) {
		this->_signed = rightSide._signed;
		std::cout << "Copy assignement " << this->_name << " has been called" << std::endl;
	}
	return *this;
}

/*
	Destructor
*/
Form::~Form() {
	std::cout << "Destructor Form " <<this->_name << " has been called." << std::endl;
}

/*
	getter function
	@return
		this->_name: the name of the class
*/
const std::string	&Form::getName(void) const {
	return this->_name;
}

/*
	getter function
	@return
		this->_gradeToSign: required grade
*/
int	Form::getGradeToSign(void) const {
	return this->_gradeToSign;
}

/*
	getter function
	@return
		this->_gradeToExec: required grade
*/
int	Form::getGradeToExec(void) const {
	return this->_gradeToExec;
}

/*
	getter function
	@return
		this->_signed: flag to verify sign
*/
bool	Form::isSigned(void) const {
	return this->_signed;
}

/*
	check sign status
	@param
		b: reference of the class Bureaucrat
*/
void	Form::beSigned(const Bureaucrat &b)
{
	if (b.getGrade() > this->_gradeToSign)
		throw GradeTooLowException();
	this->_signed = true;
}

std::ostream	&operator<<(std::ostream &out, const Form &form) {
	out << "Form: " << form.getName() << " signed: " << (form.isSigned() ? "true" : "false");
	out << " GradeToSign: " << form.getGradeToSign();
	out << " GradeToexec: " << form.getGradeToExec();

	return out;
}
