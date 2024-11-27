/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:31 by donghank          #+#    #+#             */
/*   Updated: 2024/11/23 15:49:40 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Weapon.hpp"

/*
	Constructor
	set the type of the weapon
*/
Weapon::Weapon(std::string type) {
	this->setType(type);
	// std::cout << "Constructor of the Weapon has been called" << std::endl;
}

/*
	Destructor
*/
Weapon::~Weapon() {
	// std::cout << "Destructor of the Weapon has been called" << std::endl;
}

/*
	to get the type of the weapon
*/
const std::string&	Weapon::getType() {
	return (this->type);
}

/*
	setting the type of the weapon
*/
void	Weapon::setType(std::string newType) {
	this->type = newType;
}
