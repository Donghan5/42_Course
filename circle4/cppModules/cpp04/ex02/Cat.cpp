/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:06 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:23:07 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

Cat::Cat(void): Animal("Cat") {
	std::cout << "Default constructor of " << this->type << " has been called." << std::endl;
	try {
		this->_brain = new Brain();
	}
	catch (std::bad_alloc& e) {
		std::cout << "Memory allocaton failed: " << e.what() << std::endl;
	}
}

Cat::Cat(const Cat &rightSide): Animal(rightSide), _brain(new Brain(*rightSide._brain)) {
	std::cout << "Copy constructor of " << this->type << " has been called." << std::endl;
}

Cat::~Cat(void) {
	std::cout << "Destructor of " << this->type << " has been called." << std::endl;
	delete _brain;
}

Animal	&Cat::operator=(const Animal &rightSide) {
	const Cat	*other;

	other = dynamic_cast<const Cat *>(&rightSide);
	if (other) {
		std::cout << "Cat operator= has been called" << std::endl;
		this->type = other->type;
		std::cout << "Type copied: " << this->type << "to " << other->type << std::endl;
		*this->_brain = *other->_brain;
	}
	else {
		std::cout << "Dynamic cast failed in Cat" << std::endl;
	}
	return (*this);
}

void	Cat::makeSound(void) const {
	std::cout << " * Meow~~~ * " << std::endl;
}

Brain	*Cat::getBrain(void) const {
	return (this->_brain);
}
