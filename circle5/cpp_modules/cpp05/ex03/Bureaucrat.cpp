/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bureaucrat.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:51:58 by donghank          #+#    #+#             */
/*   Updated: 2024/12/28 14:35:57 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"

/*
	Default constructor
*/
Bureaucrat::Bureaucrat(): _name("Default"), _grade(150) {
	// std::cout << "Default constructor has been called" << std::endl;
}

/*
	Type constructor
	@param
		name: name of the class
		grade: grade of the name
*/
Bureaucrat::Bureaucrat(const std::string &name, int grade): _name(name) {
	if (grade < 1)
		throw Bureaucrat::GradeTooHighException();
	else if (grade > 150)
		throw Bureaucrat::GradeTooLowException();
	this->_grade = grade;
}

/*
	copy constructor
	@param
		rightSide: copy src
*/
Bureaucrat::Bureaucrat(const Bureaucrat &rightSide): _name(rightSide._name), _grade(rightSide._grade) {
	std::cout << "Copy constructor " << this->_name << " has been called" << std::endl;
}

/*
	Destructor
*/
Bureaucrat::~Bureaucrat() {
	std::cout << "Destructor " << this->_name << " has been called" << std::endl;
}

/*
	Copy assingement
	@param
		rightSide: copy src
	@return
		this: self-pointer of the class
*/
Bureaucrat	&Bureaucrat::operator=(const Bureaucrat &rightSide) {
	if (this != &rightSide) {
		std::cout << "Copy assignement has been called" << std::endl;
		this->_grade = rightSide._grade;
	}
	return *this;
}

/*
	increment grade --> lowest grade is 150
*/
void	Bureaucrat::incrementGrade(void) {
	if (this->_grade <= Bureaucrat::maxGrade)
		throw GradeTooHighException();
	this->_grade--;
}

/*
	decrement grade --> hight=est grade is 1
*/
void	Bureaucrat::decrementGrade(void) {
	if (this->_grade >= Bureaucrat::minGrade)
		throw GradeTooLowException();
	this->_grade++;
}

/*
	getter
	@return
		this->_name: type of class
*/
const std::string	&Bureaucrat::getName(void) const {
	return this->_name;
}

/*
	getter
	@return
		this->_grade: grade
*/
int	Bureaucrat::getGrade(void) const {
	return this-> _grade;
}

const char	*Bureaucrat::GradeTooHighException::what() const throw() {
	return "Grade is too high";
}

const char	*Bureaucrat::GradeTooLowException::what() const throw() {
	return "Grade is too low";
}

/*
	signForm function
	@param
		From of the sign
*/
void	Bureaucrat::signForm(AForm &form) {
	try {
		form.beSigned(*this);
		std::cout << "Bureaucrat " << this->_name << " successfully signed ";
		std::cout << "the form " << form.getName() << "." << std::endl;
	}
	catch (std::exception &e) {
		std::cout << *this << " can't sign. [REASON] ";
		std::cerr << e.what() << std::endl;
	}
}

void	Bureaucrat::executeForm(const AForm& form) const {
	try {
		form.execute(*this);
	}
	catch (std::exception &e) {
		std::cerr << *this << " can't sign " << form.getName() << " because " << e.what() << std::endl;
	}
}

/*
	@param
		str: string to print
		bureauCrat: class to take name and grade
*/
std::ostream	&operator<<(std::ostream &str, Bureaucrat const &bureaucrat) {
	str << bureaucrat.getName() << ", bureaucrat grade " << bureaucrat.getGrade();
	return str;
}

