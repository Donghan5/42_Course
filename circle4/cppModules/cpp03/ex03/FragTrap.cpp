/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:19:51 by donghank          #+#    #+#             */
/*   Updated: 2024/11/28 13:56:04 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FragTrap.hpp"

/*
	default constructor
*/
FragTrap::FragTrap() {
	this->_name = "frag";
	this->_hitPoints = 100;
	this->_energyPoints = 100;
	this->_attackDamage = 20;
	std::cout << "FragTrap " << this->_name << " default constructor called." << std::endl;
}

/*
	type constructor
*/
FragTrap::FragTrap(std::string name): ClapTrap(name) {
	this->_hitPoints = 100;
	this->_energyPoints = 100;
	this->_attackDamage = 20;
	std::cout << "FragTrap " << this->_name << " constructed." << std::endl;
}

/*
	destructor
*/
FragTrap::~FragTrap() {
	std::cout << "FragTrap " << this->_name << " destructed." << std::endl;
}

/*
	copy constructor
	@param
		rightSide: copy object
*/
FragTrap::FragTrap(const FragTrap &rightSide): ClapTrap(rightSide) {
	std::cout << "FragTrap " << this->_name << " Copy constructor called." << std::endl;
}

/*
	copy assignement
	@param
		rightSide: copy object
*/
FragTrap	&FragTrap::operator=(const FragTrap &rightSide) {
	if (this != &rightSide) {
		std::cout << "FragTrap " << this->_name << " Assignement has been called." << std::endl;
		ClapTrap::operator=(rightSide);
	}
	return *this;
}

/*
	attack function
	@param
		target: target person
*/
void	FragTrap::attack(const std::string &target) {
	if (_energyPoints <= 0) {
		std::cout << "FragTrap " << this->_name << " have no more energy" << std::endl;
		return ;
	}
	std::cout << "FragTrap " << this->_name << " attacks " << target << ", causing " << _attackDamage << " points of damage!" << std::endl;
	_energyPoints -= 1;
}

/*
	special function of fragtrap
*/
void	FragTrap::highFiveGuys(void) {
	std::cout << "FragTrap " << this->_name << " highfive, keep going guys!" << std::endl;
}
