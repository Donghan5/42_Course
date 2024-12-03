/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiamondTrap.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:19:35 by donghank          #+#    #+#             */
/*   Updated: 2024/12/03 12:24:51 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DiamondTrap.hpp"

/*
	Default constructor
*/
DiamondTrap::DiamondTrap(): ClapTrap(), ScavTrap(), FragTrap() {
	this->_hitPoints = FragTrap::_hitPoints;
	this->_energyPoints = ScavTrap::_energyPoints;
	this->_attackDamage = FragTrap::_attackDamage;
	std::cout << "DiamondTrap " << this->_name << " default constructor has been called" << std::endl;
}

/*
	The reason to put _clap_name --> to verify ClapTrap and DiamondTrap
	@param
		name: name of DiamondTrap object
*/
DiamondTrap::DiamondTrap(std::string name): ClapTrap(name), ScavTrap(name), FragTrap(name) {
	this->_name = name;
	this->ClapTrap::_name = name + "_clap_name";
	this->_hitPoints = FragTrap::_hitPoints;
	this->_energyPoints = ScavTrap::_energyPoints;
	this->_attackDamage = FragTrap::_attackDamage;
	std::cout << "DiamondTrap " << this->_name << " constructor has been called" << std::endl;
}

/*
	copy constructor
	@param
		rightSide: copy object
*/
DiamondTrap::DiamondTrap(const DiamondTrap &rightSide): ClapTrap(rightSide), ScavTrap(rightSide), FragTrap(rightSide) {
	std::cout << "DiamondTrap " << this->_name << " copy constructor has been called." << std::endl;
}

/*
	copy assignement
	@param
		rightSide: copy object
*/
DiamondTrap	&DiamondTrap::operator=(const DiamondTrap &rightSide) {
	if (this != &rightSide) {
		this->_name = rightSide._name;
		this->ClapTrap::_name = rightSide.ClapTrap::_name;
		this->_energyPoints = rightSide._energyPoints;
		this->_attackDamage = rightSide._attackDamage;
		this->_hitPoints = rightSide._hitPoints;
	}
	return (*this);
}

/*
	Destructor
*/
DiamondTrap::~DiamondTrap() {
	std::cout << "DiamondTrap " << this->_name << " destructed." << std::endl;
}

/*
	print the name of the personnage
*/
void	DiamondTrap::whoAmI(void) {
	std::cout << "DiamondTrap name: " << this->_name << std::endl;
	std::cout << "ClapTrap name: " << ClapTrap::_name << std::endl;
	// std::cout << "ScavTrap name: " << ScavTrap::_name << std::endl;
	// std::cout << "FragTrap name: " << FragTrap::_name << std::endl;
}
