/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:16:28 by donghank          #+#    #+#             */
/*   Updated: 2024/11/28 18:02:02 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

/*
	default constructor
*/
ClapTrap::ClapTrap(): _name("default"), _hitPoints(10), _energyPoints(10), _attackDamage(0) {
	std::cout << "ClapTrap" << _name << " default constructor created" << std::endl;
}

/*
	Type constructor
*/
ClapTrap::ClapTrap(std::string name): _name(name), _hitPoints(10), _energyPoints(10), _attackDamage(0) {
	std::cout << "ClapTrap " << _name << " constructed" << std::endl;
}

/*
	destructor
*/
ClapTrap::~ClapTrap() {
	std::cout << "ClapTrap " << _name << " destructed." << std::endl;
}

/*
	copy constructor
	@param
		rightSide: copy object
*/
ClapTrap::ClapTrap(const ClapTrap &rightSide) {
	std::cout << "Copy constructor is called" << std::endl;
	*this = rightSide;
}

/*
	@param
		rightSide = copy object
*/
ClapTrap	&ClapTrap::operator=(const ClapTrap &rightSide) {
	if (this != &rightSide) {
		this->_name = rightSide._name;
		this->_hitPoints = rightSide._hitPoints;
		this->_energyPoints = rightSide._energyPoints;
		this->_attackDamage = rightSide._attackDamage;
	}
	return *this;
}

/*
	@param
		target: target person
*/
void	ClapTrap::attack(const std::string &target) {
	if (_energyPoints <= 0) {
		std::cout << _name << " have no more energy" << std::endl;
		return ;
	}
	std::cout << _name << " attacks " << target << ", causing " << _attackDamage << " points of damage!" << std::endl;
	_energyPoints -= 1;
}

/*
	@param
		amount: amount of the damage
*/
void	ClapTrap::takeDamage(unsigned int amount) {
	if (_hitPoints <= amount) {
		std::cout << "ClapTrap " << _name << " is dead!" << std::endl;
		return ;
	}
	std::cout << "ClapTrap " << _name << " takes " << amount << " damage!" << std::endl;
	_hitPoints -= amount;
}

/*
	reparing cost is 1 energy point
	@param
		amount: reparing amount
*/
void	ClapTrap::beRepaired(unsigned int amount) {
	if (_energyPoints <= 0) {
		std::cout << "ClapTrap " << _name << " is out of energy!" << std::endl;
		return ;
	}
	std::cout << "ClapTrap " << _name << " repaire itself " << amount << " hit points!" << std::endl;
	_energyPoints -= 1; // repare cost
	_hitPoints += amount;
}
