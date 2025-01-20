/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:51:52 by donghank          #+#    #+#             */
/*   Updated: 2024/12/28 14:35:37 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AForm.hpp"

/*
	Exception function
*/
const char	*AForm::GradeTooHighException::what() const throw() {
	return "Grade is too high";
}

/*
	Exception function
*/
const char	*AForm::GradeTooLowException::what() const throw() {
	return "Grade is too low";
}

/*
	Exception function
*/
const char	*AForm::InvalidFormException::what() const throw() {
	return "Invalid form";
}

/*
	Default constructor
*/
AForm::AForm(): _name("Default"), _gradeToSign(1), _gradeToExec(1), _signed(false) {
	std::cout << "Default constructor has been called" << std::endl;
}

/*
	Type(name) constructor
	@param
		name: name of the class
		gradeToSign: required grade to sign
		gradeToExec: required grad to execution
*/
AForm::AForm(const std::string &name, int gradeToSign, int gradeToExec): _name(name), _gradeToSign(gradeToSign), _gradeToExec(gradeToExec) {
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
AForm::AForm(const AForm &rightSide): _name(rightSide._name), _gradeToSign(rightSide._gradeToSign), _gradeToExec(rightSide._gradeToExec), _signed(rightSide._signed) {
	std::cout << "Copy constructor " << this->_name << " has been called" << std::endl;
}

/*
	Copy assingment
	@param
		rightSide: copy obj
*/
AForm	&AForm::operator=(const AForm &rightSide) {
	if (this != &rightSide) {
		this->_signed = rightSide._signed;
		std::cout << "Copy assignement " << this->_name << " has been called" << std::endl;
	}
	return *this;
}

/*
	Destructor
*/
AForm::~AForm() {
	std::cout << "Destructor Form " << this->_name << " has been called." << std::endl;
}

/*
	getter function
	@return
		this->_name: the name of the class
*/
std::string	AForm::getName(void) const {
	return this->_name;
}

/*
	getter function
	@return
		this->_gradeToSign: required grade
*/
int	AForm::getGradeToSign(void) const {
	return this->_gradeToSign;
}

/*
	getter function
	@return
		this->_gradeToExec: required grade
*/
int	AForm::getGradeToExec(void) const {
	return this->_gradeToExec;
}

/*
	getter function
	@return
		this->_signed: flag to verify sign
*/
bool	AForm::isSigned(void) const {
	return this->_signed;
}

/*
	check sign status
	@param
		b: reference of the class Bureaucrat
*/
void	AForm::beSigned(const Bureaucrat &b)
{
	if (b.getGrade() > this->_gradeToSign)
		throw GradeTooLowException();
	this->_signed = true;
}

/*
	check the validity of the sign
*/
void	AForm::checkExecutability(const Bureaucrat &executor) const {
	if (executor.getGrade() > _gradeToExec)
		throw GradeTooLowException();
	if (!this->_signed)
		throw InvalidFormException();
}

std::ostream	&operator<<(std::ostream &out, const AForm &aform) {
	out << "Form: " << aform.getName() << " signed: " << (aform.isSigned() ? "true" : "false");
	out << " GradeToSign: " << aform.getGradeToSign();
	out << " GradeToexec: " << aform.getGradeToExec();

	return out;
}
