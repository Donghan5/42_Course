/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 20:22:27 by donghank          #+#    #+#             */
/*   Updated: 2025/01/22 12:13:07 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

/*
	generate the class A, B and C
*/
Base *generate(void) {
	int ranGen = rand() % 3;

	switch (ranGen) {
		case 0:
			return (new A());
		case 1:
			return (new B());
		case 2:
			return (new C());
		default:
			return NULL;
	}
}

/*
	pointer identifier
	dynamic_cast --> upcasting and downcasting (between parent class and child class)
	the case fail --> return nullptr
*/
void identify(Base* p) {
	if (dynamic_cast<A* >(p))
		std::cout << "A" << std::endl;
	else if (dynamic_cast<B* >(p))
		std::cout << "B" << std::endl;
	else if (dynamic_cast<C* >(p))
		std::cout << "C" << std::endl;
}

/*
	reference identifier
	the case fail --> throw std::bad_cast
*/
void identify(Base& p) {
	Base base;
	try {
		base = dynamic_cast<A& >(p);
		std::cout << "A" << std::endl;
	}
	catch (const std::exception &e) {}
	try {
		base = dynamic_cast<B& >(p);
		std::cout << "B" << std::endl;
	}
	catch (const std::exception &e) {}
	try {
		base = dynamic_cast<C& >(p);
		std::cout << "C" << std::endl;
	}
	catch (const std::exception &e) {}
}

int main(void) {
	Base	*base;

	srand(time(NULL)); // init
	std::cout << "=============== POINTER TEST ===============" << std::endl;

	std::cout << "\n=============== TEST 1 ===============" << std::endl;
	base = generate();
	identify(base);
	delete base;

	std::cout << "=============== TEST 2 ===============" << std::endl;
	base = generate();
	identify(base);
	delete base;

	std::cout << "=============== TEST 3 ===============" << std::endl;
	base = generate();
	identify(base);
	delete base;

	std::cout << "\n=============== REF TEST ===============" << std::endl;

	std::cout << "\n=============== TEST 1 ===============" << std::endl;
	base = generate();
	identify(*base);
	delete base;

	std::cout << "=============== TEST 2 ===============" << std::endl;
	base = generate();
	identify(*base);
	delete base;

	std::cout << "=============== TEST 3 ===============" << std::endl;
	base = generate();
	identify(*base);
	delete base;

	return 0;
}
