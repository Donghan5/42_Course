/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:13:17 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 21:08:23 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

/*
	Constructor
*/
Zombie::Zombie(std::string name) {
	this->_name = name;
}

/*
	Destructor
*/
Zombie::~Zombie() {
	std::cout << "Zombie: " << this->_name << " has been destroyed" << std::endl;
}

/*
	Zombie announce fuction, just print Zombie name and Braiiiiinnnnnzzz...
*/
void	Zombie::announce() {
	std::cout << this->_name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}
