/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:20:01 by donghank          #+#    #+#             */
/*   Updated: 2024/11/30 13:47:33 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DiamondTrap.hpp"

/*
	Call constructor
	1. ClapTrap (parent class)
	2. ScavTrap / FragTrap
	3. DiamondTrap
	Call destructor
	1. DiamondTrap
	2. ScavTrap / FragTrap
	3. ClapTrap (parent class)

	note: The output name (John_clap_name) is applied according to the parent class name change rule (adding _clap_name)
*/
int	main(void) {
	DiamondTrap	John("John");
	// DiamondTrap	Nick("Nick");
	// DiamondTrap	Cole("Cole");

	John.attack("BOB");
	John.attack("Alice");
	John.attack("Vincent");
	John.beRepaired(10);
	John.takeDamage(10);
	John.guradGate();
	John.highFiveGuys();
	John.whoAmI();

	// Nick part
	// Nick.attack("BOB");
	// Nick.attack("Alice");
	// Nick.attack("Vincent");
	// Nick.beRepaired(10);
	// Nick.takeDamage(10);
	// Nick.guradGate();
	// Nick.highFiveGuys();
	// Nick.whoAmI();

	// Just to check assigning operator
	// std::cout << "Assigning Cole to John" << std::endl;
	// John = Cole;
	return 0;
}
