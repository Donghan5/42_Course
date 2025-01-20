/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:51:32 by donghank          #+#    #+#             */
/*   Updated: 2025/01/20 11:34:53 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"

/*
	To treat the exception, have to use std::exception &e
	This action isn't use dynamic allocate
*/
int main(void) {
	// increment part
	std::cout << "====== This is increment test ======" << std::endl;
	try {
		Bureaucrat	bureauCrat("John", 4);

		// Bureaucrat	bureauCrat("John", 1); // exception test
		std::cout << "-- before increment operation --" << std::endl;
		std::cout << bureauCrat << std::endl;

		std::cout << "-- after increment operation --" << std::endl;
		bureauCrat.incrementGrade();
		std::cout << bureauCrat << std::endl;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	// decrement part
	// std::cout << "====== This is decrement test ======" << std::endl;
	// 	try {
	// 	Bureaucrat bureauCrat("Bob", 150);
	// 	Bureaucrat bureauCrat("Bob", 30);

	// std::cout << "-- before decrement operation --" << std::endl;
	// 	std::cout << bureauCrat << std::endl;

	// std::cout << "-- after decrement operation --" << std::endl;
	// 	bureauCrat.decrementGrade();
	// std::cout << bureauCrat << std::endl;
	// }
	// catch (const std::exception &e) {
	// 	std::cerr << e.what() << std::endl;
	// }

	return 0;
}
