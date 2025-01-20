/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 11:45:17 by donghank          #+#    #+#             */
/*   Updated: 2024/12/28 14:51:26 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Intern.hpp"

int	main(void) {
	std::cout << "================ Let's put to the test ================" << std::endl;
	std::cout << "\n================ RobotomyRequestForm ================" << std::endl;
	{
		Intern	someRandomIntern;
		AForm	*af;
		// af = someRandomIntern.makeForm("rotobo", "Benedit");
		af = someRandomIntern.makeForm("robotomy request", "Benendit");
		delete af;
	}
	std::cout << "================ ShrubberyCreationForm ================" << std::endl;
	{
		Intern	Tom;
		AForm	*sb;
		// af = Tom.makeForm("shburrry", "Tom");
		sb = Tom.makeForm("shrubbery creation", "Tom");
		delete sb;
	}
	std::cout << "================ PresidentialPardonForm ================" << std::endl;
	{
		Intern	Bob;
		AForm	*pp;
		// pp = Bob.makeForm("presidnet cerasd", "Bob");
		pp = Bob.makeForm("presidential pardon", "Bob");
		delete pp;
	}
	std::cout << "================ End the tests ================" << std::endl;
	std::cout << "================ Test with Bureaucrat ================" << std::endl;
	try {
		Bureaucrat	bc("Kris", 2);
		RobotomyRequestForm formOne("robotomy");
		ShrubberyCreationForm formTwo("surubbery");
		PresidentialPardonForm formThree("presidential");

		std::cout << "---------- formOne (RobotomyRequest) ----------" << std::endl;
		bc.signForm(formOne);
		bc.executeForm(formOne);
		std::cout << "---------- formTwo (ShrubberyCreation) ----------" << std::endl;
		bc.signForm(formTwo);
		bc.executeForm(formTwo);
		std::cout << "---------- formThree (PresidentialPardon) ----------" << std::endl;
		bc.signForm(formThree);
		bc.executeForm(formThree);
		std::cout << "---------- FINISH ----------" << std::endl;
	}
	catch (const std::exception &e) {
		std::cerr << "[EXCEPTION] " << e.what() << std::endl;
	}
	return 0;
}
