/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:20:45 by donghank          #+#    #+#             */
/*   Updated: 2024/12/06 15:08:28 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"

/*
	Constructor
*/
Dog::Dog(void): Animal("Dog") {
	std::cout << "Default constructor of " << this->type << " has been called." << std::endl;
}

/*
	copy constructor
	@param
		rightSide: copy objs
*/
Dog::Dog(const Dog &rightSide): Animal(rightSide) {
	std::cout << "Copy assignement of " << this->type << " has been called." << std::endl;
}

/*
	destructor
*/
Dog::~Dog(void) {
	std::cout << "Destructor of " << this->type << " has been called." << std::endl;
}

/*
	function required
*/
void	Dog::makeSound(void) const {
	std::cout << " * Barking bow wow!! *" << std::endl;
}
