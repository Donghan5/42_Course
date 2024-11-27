/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:23 by donghank          #+#    #+#             */
/*   Updated: 2024/11/23 15:48:48 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanB.hpp"

/*
	constructor
	name --> init with list
*/
HumanB::HumanB(std::string name): _name(name) {
	this->_weapon = NULL;
	// std::cout << "Constructor of HumanB has been called" << std::endl;
}

/*
	destructor
*/
HumanB::~HumanB() {
	// std::cout << "Destructor of HumanB has been called" << std::endl;
}

/*
	setter (setting the weapon of HumanB)
*/
void	HumanB::setWeapon(Weapon &weapon) {
	this->_weapon = &weapon;
}

/*
	attack function
	In this case, HumanB --> two cases
		1. HumanB haven't weapon
		2. HumanB have a weapon
*/
void	HumanB::attack() const {
	if (this->_weapon != NULL && this->_weapon->getType() != "") {
		std::cout << _name << " attack with their " << this->_weapon->getType() << std::endl;
	}
	else
		std::cout << _name << " doesn't have a weapon to attack " << std::endl;
}
