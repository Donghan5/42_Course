/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:51:48 by donghank          #+#    #+#             */
/*   Updated: 2025/01/20 11:37:57 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Form.hpp"
#include "Bureaucrat.hpp"

/*
	To treat the exception, always have to use std::exception &e
	This action isn't use dynamic allocate
*/
int main(void) {
	// Multiple Form tests
	std::cout << "====== Multiple Form tests ======" << std::endl;
	try {
		std::cout << "This one to create grade sign 0 (exception)" << std::endl;
		Form f("KE113", 0, 1);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << std::endl;
	try {
		std::cout << "This one to create grade sign 3" << std::endl;
		Form f("KS112", 2, 3);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << std::endl;
	try {
		std::cout << "This one to create grade sign 151 (exception)" << std::endl;
		Form f("KA132", 151, 1);
	}
	catch (const std::exception &e){
		std::cerr << e.what() << std::endl;
	}
	std::cout << std::endl;
	try {
		std::cout << "This one to create grade exec 30" << std::endl;
		Form f("CB120", 1, 30);
	}
	catch (const std::exception &e){
		std::cerr << e.what() << std::endl;
	}
	std::cout << std::endl;
	try {
		std::cout << "This one to create grade exec 151 (exception)" << std::endl;
		Form f("SS212", 1, 151);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "=================================" << std::endl;
	std::cout << "======= With Bureaucrat 1 =======" << std::endl;
	try {
		Bureaucrat	Odin("Odin", 1);
		Form		f("LE110", 150, 150);
		Odin.signForm(f);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "=================================" << std::endl;

	std::cout << "======= With Bureaucrat 2 (exception) =======" << std::endl;
	try {
		Bureaucrat	Thor("Thor", 150);
		Form		f("LS123", 1, 1);
		Thor.signForm(f);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "=================================" << std::endl;

	return 0;
}
