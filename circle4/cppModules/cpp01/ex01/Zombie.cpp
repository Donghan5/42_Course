/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:13:31 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 21:09:46 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

/*
	constructor
*/
Zombie::Zombie(void) {

}

/*
	destructor
*/
Zombie::~Zombie() {
	std::cout << "Zombie: " << this->_name << " has been destroyed" << std::endl;
}

/*
	basic function
*/
void	Zombie::announce() {
	std::cout << this->_name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

/*
	initialize the new Zombie name
	also possible to use const std::string &name --> more efficient
*/
void	Zombie::makeZombie(std::string name) {
	this->_name = name;
}
