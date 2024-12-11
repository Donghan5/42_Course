/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:12 by donghank          #+#    #+#             */
/*   Updated: 2024/12/06 15:50:04 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"

/*
	Default constructor
*/
Dog::Dog(void): Animal("Dog") {
	std::cout << "Default constructor of " << this->type << " has been called." << std::endl;
	try {
		this->_brain = new Brain();
	}
	catch(std::bad_alloc& e) {
		std::cout << "Memory allocation failed: " << e.what() << std::endl;
	}
}

/*
	Copy constructor
	@param
		rightSide: copy objs
*/
Dog::Dog(const Dog &rightSide): Animal(rightSide), _brain(new Brain(*rightSide._brain)) {
	std::cout << "Copy assignement of " << this->type << " has been called." << std::endl;
}

/*
	Destructor
*/
Dog::~Dog(void) {
	std::cout << "Destructor of " << this->type << " has been called." << std::endl;
	delete _brain;
}


/*
	copy assignement
	@param
		rightSide: copy objs
		other: copy objs (temp)
	@return
		this: self pointer of the class
*/
Animal	&Dog::operator=(const Animal &rightSide) {
	const Dog	*other;

	other = dynamic_cast<const Dog *>(&rightSide);
	if (other) {
		std::cout << "Dog operator= has been called" << std::endl;
		this->type = other->type;
		std::cout << "Type copied: " << this->type << "to " << other->type << std::endl;
		*this->_brain = *other->_brain;
	}
	else {
		std::cout << "Dynamic cast failed in Dog" << std::endl;
	}
	return (*this);
}

/*
	required function
*/
void	Dog::makeSound(void) const {
	std::cout << " * Barking bow wow!! * " << std::endl;
}

/*
	getter
	@return
		this->_brain: brain of the Dog
*/
Brain	*Dog::getBrain(void) const {
	return (this->_brain);
}
