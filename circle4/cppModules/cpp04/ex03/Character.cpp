/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Character.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:53 by donghank          #+#    #+#             */
/*   Updated: 2024/12/10 12:45:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Character.hpp"

/*
	Default constructor
*/
Character::Character(): _name("Default") {
	std::cout << "Default constructor has been called" << std::endl;
	for (int i = 0; i < 4; i++) {
		this->_inventory[i] = NULL;
	}
	// std::cout << "Character " << this->_name << " constructor has been called" << std::endl;
}

/*
	Type constructor
	@param
		name: name of the character
*/
Character::Character(const std::string &name): _name(name) {
	// std::cout << "Type constructor of the Character has been called." << std::endl;
	for (int i = 0; i < 4; i++) {
		this->_inventory[i] = NULL;
	}
	// std::cout << "Character " << this->_name << " constructor has been called" << std::endl;
}

/*
	Copy constructor
	@param
		rightSide: copy obj
*/
Character::Character(const Character &rightSide) {
	// std::cout << "Copy-constructor has been called" << std::endl;
	*this = rightSide;
}

/*
	Destructor
*/
Character::~Character() {
	// std::cout << "Destructor of the Character has been called." << std::endl;
	for (int i = 0; i < 4; i++) {
		if (this->_inventory[i])
			delete this->_inventory[i];
	}
}

/*
	Copy assigngment
	@param
		rightSide:
*/
Character	&Character::operator=(const Character &rightSide) {
	if (this != &rightSide) {
		this->_name = rightSide._name;
		for (int i = 0; i < 4; i++) {
			this->_inventory[i] = rightSide._inventory[i];
		}
	}
	return (*this);
}

std::string const	&Character::getName(void) const {
	return (this->_name);
}

void	Character::setName(const std::string &name) {
	this->_name = name;
}

void	Character::equip(AMateria *m) {
	for (int i = 0; i < 4; i++) {
		if (this->_inventory[i] == NULL) {
			this->_inventory[i] = m;
			// std::cout << "Character " << this->_name << " equiped" << std::endl;
			return ;
		}
	}
	std::cout << "Character " << this->_name << " can't equiped" << std::endl;
}

void	Character::unequip(int idx) {
	if (this->_inventory[idx]) {
		// delete this->_inventory[idx];
		this->_inventory[idx] = NULL;
	}
	else {
		std::cout << "Character " << this->_name << " fail to unequipe" << std::endl;
	}
}

void	Character::use(int idx, ICharacter &target) {
	if (this->_inventory[idx]) {
		this->_inventory[idx]->use(target);
	}
	else {
		std::cout << "Character " << this->_name << " can't use this" << std::endl;
	}
}
