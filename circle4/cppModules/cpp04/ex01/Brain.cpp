/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:22:11 by donghank          #+#    #+#             */
/*   Updated: 2024/12/06 15:25:55 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Brain.hpp"

/*
	Default constructor
*/
Brain::Brain(void): _size(0) {
	std::cout << "Default constructor of Brain has been called." << std::endl;
}

/*
	Copy constructor
	@param
		rightSide: copy obj
*/
Brain::Brain(const Brain &rightSide) {
	std::cout << "Copy-constructor of Brain has been called." << std::endl;
	*this = rightSide;
}

/*
	Copy assingement
	@param
		rightSide: copy objs
	@return
		this: self-pointer of the class
*/
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

/*
	destructor
*/
Brain::~Brain(void) {
	std::cout << "Destructor of Brain has been called." << std::endl;
}

/*
	getting the ideas in specific index
	@param
		idx: index of ideas
	@return
		this->_ideas[idx]: specific ideas
*/
const std::string	&Brain::getIdeas(unsigned int idx) const {
	return (this->_ideas[idx]);
}

/*
	Adding ideas
	@param
		idea: ideas which want to add
	@return
		true: adding is successfully done
		false: Failurem (the case already exist, no more place etc...)
*/
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
