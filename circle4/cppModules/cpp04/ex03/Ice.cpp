/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ice.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:24:07 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:24:08 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ice.hpp"

Ice::Ice(): AMateria("ice") {
	// std::cout << "Default constructor has been called" << std::endl;
}

Ice::Ice(const Ice &rightSide): AMateria("ice") {
	// std::cout << "Copy constructor has been called" << std::endl;
	*this = rightSide;
}

Ice::~Ice() {
	// std::cout << "Destructor has been called" << std::endl;
}

const Ice	&Ice::operator=(const Ice &rightSide) {
	if (this != &rightSide)
		this->_type = rightSide._type;
	return (*this);
}

AMateria	*Ice::clone(void) const {
	return (new Ice(*this));
}

void	Ice::use(ICharacter &target) {
	std::cout << "*  shoots an ice bolt at " << target.getName() << " *" << std::endl;
}
