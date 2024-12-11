/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:22:18 by donghank          #+#    #+#             */
/*   Updated: 2024/12/06 15:33:01 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"

/*
	Default constructor of Cat
*/
Cat::Cat(void): Animal("Cat") {
	std::cout << "Default constructor of " << this->type << " has been called." << std::endl;
	try {
		this->_brain = new Brain();
	}
	catch (std::bad_alloc& e) {
		std::cout << "Memory allocaton failed: " << e.what() << std::endl;
	}
}

/*
	Copy assignement
		@param
			rightSide: copy objs
*/
Cat::Cat(const Cat &rightSide): Animal(rightSide), _brain(new Brain(*rightSide._brain)) {
	std::cout << "Copy constructor of " << this->type << " has been called." << std::endl;
}

/*
	Destructor
*/
Cat::~Cat(void) {
	std::cout << "Destructor of " << this->type << " has been called." << std::endl;
	delete _brain;
}

/*
	Copy assignement
	@param
		rightSide: copy objs
		other: copy objs (temporary)
	@return
		this: self-pointer of the class
*/
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

/*
	required function
*/
void	Cat::makeSound(void) const {
	std::cout << " * Meow~~~ * " << std::endl;
}

/*
	getter
	@return
		this->_brain: brain of the class
*/
Brain	*Cat::getBrain(void) const {
	return (this->_brain);
}
