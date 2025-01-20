/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:52:07 by donghank          #+#    #+#             */
/*   Updated: 2025/01/20 11:41:49 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"

/*
	To treat the exception, always have to use std::exception &e
	This action isn't use dynamic allocate
*/
int main(void) {
	try {
			Bureaucrat bureaucrat("ash", 1); // error with 200
			// Bureaucrat bureaucrat("ash", 200); // error case
			ShrubberyCreationForm form1("Shrubbery");
			RobotomyRequestForm form2("Robotomy");
			PresidentialPardonForm form3("President");

			std::cout << "\n--------------- ShrubberyCreationForm test ---------------" << std::endl;
			bureaucrat.signForm(form1);
			bureaucrat.executeForm(form1);
			std::cout << "\n--------------- RobotomyRequestForm test (success rate: 50%) ---------------" << std::endl;
			bureaucrat.signForm(form2);
			bureaucrat.executeForm(form2);
			bureaucrat.executeForm(form2);
			bureaucrat.executeForm(form2);
			bureaucrat.executeForm(form2);
			std::cout << "\n--------------- PresidentialSignForm test ---------------" << std::endl;
			bureaucrat.signForm(form3);
			bureaucrat.executeForm(form3);
	} catch (const std::exception &e) {
		std::cerr << "[EXECEPTION] " << e.what() << std::endl;
	}
	return 0;
}
