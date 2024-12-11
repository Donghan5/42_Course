/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:01 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:23:02 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Brain.hpp"

Brain::Brain(void): _size(0) {
	std::cout << "Default constructor of Brain has been called." << std::endl;
}

Brain::Brain(const Brain &rightSide) {
	std::cout << "Copy-constructor of Brain has been called." << std::endl;
	*this = rightSide;
}

Brain	&Brain::operator=(const Brain &rightSide) {
	std::cout << "Calling Brain operator...." << std::endl;
	if (this != &rightSide) {
		std::cout << "Copy-assingnement of Brain has been called." << std::endl;
		this->_size = rightSide._size;
		for (unsigned int i = 0; i < rightSide._size; i++) {
			this->_ideas[i] = rightSide._ideas[i];
			std::cout << "Copied idea[" << i << "]: " << this->_ideas[i] << std::endl;
		}
	}
	return (*this);
}

Brain::~Brain(void) {
	std::cout << "Destructor of Brain has been called." << std::endl;
}

const std::string	&Brain::getIdeas(unsigned int idx) const {
	return (this->_ideas[idx]);
}

bool	Brain::addIdeas(std::string idea) {
	if (_size == 100) {
		std::cout << "No more place!" << std::endl;
		return false;
	}
	for (unsigned int i = 0; i < _size; i++) {
		if (this->_ideas[i] == idea) {
			std::cout << "Already exists" << std::endl;
			return false;
		}
	}
	this->_ideas[_size++] = idea;
	return true;
}
