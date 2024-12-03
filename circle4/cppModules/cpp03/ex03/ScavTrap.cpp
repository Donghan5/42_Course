/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:20:06 by donghank          #+#    #+#             */
/*   Updated: 2024/11/28 13:55:29 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"

/*
	default constructor
*/
ScavTrap::ScavTrap() {
	this->_name = "scav";
	this->_hitPoints = 100;
	this->_energyPoints = 50;
	this->_attackDamage = 20;
	std::cout << "ScavTrap " << this->_name << " default constructor called." << std::endl;
}

/*
	type constructor
*/
ScavTrap::ScavTrap(std::string name): ClapTrap(name) {
	this->_hitPoints = 100;
	this->_energyPoints = 50;
	this->_attackDamage = 20;
	std::cout << "ScavTrap " << this->_name << " constructed." << std::endl;
}

/*
	Destructor
*/
ScavTrap::~ScavTrap() {
	std::cout << "ScavTrap " << this->_name << " destructed." << std::endl;
}

/*
	copy constructor
	@param
		rightSide: copy object
*/
ScavTrap::ScavTrap(const ScavTrap &rightSide): ClapTrap(rightSide) {
	std::cout << "ScavTrap " << this->_name << " Copy consturctor called." << std::endl;
}

/*
	copy assignement
	@param
		rightSide: copy object
*/
ScavTrap	&ScavTrap::operator=(const ScavTrap &rightSide) {
	if (this != &rightSide) {
		std::cout << "ScavTrap " << this->_name << " Assignement called" << std::endl;
		ClapTrap::operator=(rightSide);
	}
	return *this;
}

/*
	attack function
	@param
		target: target person
*/
void	ScavTrap::attack(const std::string &target) {
	if (_energyPoints <= 0) {
		std::cout << "ScavTrap " << this->_name << " have no more energy" << std::endl;
		return ;
	}
	std::cout << "ScavTrap " << this->_name << " attacks " << target << ", causing " << _attackDamage << " points of damage!" << std::endl;
	_energyPoints -= 1;
}

/*
	special function of fragtrap
*/
void	ScavTrap::guradGate(void) {
	std::cout << "ScavTrap " << this->_name << " is in mode Gate Keeper" << std::endl;
}
