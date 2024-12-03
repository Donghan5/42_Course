/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:18:53 by donghank          #+#    #+#             */
/*   Updated: 2024/11/30 12:58:14 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FragTrap.hpp"

/*
	Call constructor
	1. ClapTrap (parent class)
	2. ScavTrap
	Call destructor
	1. ScavTrap
	2. ClapTrap (parent class)
*/
int	main(void) {
	FragTrap	John("John");
	// FragTrap	Nick("Nick");
	// FragTrap	Cole("Cole");

	John.attack("BOB");
	John.attack("Alice");
	John.attack("Vincent");
	John.highFiveGuys();

	// Nick part
	// Nick.attack("BOB");
	// Nick.attack("Alice");
	// Nick.attack("Vincent");
	// Nick.highFiveGuys();

	// just to check assigning operator
	// std::cout << "Assigning Cole to John" << std::endl;
	// John = Cole;

	return 0;
}
