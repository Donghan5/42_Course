/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiamondTrap.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:19:43 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:19:44 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIAMONDTRAP_HPP
#define DIAMONDTRAP_HPP

#include "ClapTrap.hpp"
#include "ScavTrap.hpp"
#include "FragTrap.hpp"

/*
	Keyword: virtual
	Purpose: To resolve the Diamond Problem in multiple inheritance.

	Diagram:
			ClapTrap
		   /       \
	 ScavTrap    FragTrap
		   \       /
		  DiamondTrap

	Explanation:
	- In multiple inheritance, the same parent class (ClapTrap) is inherited
	  multiple times via different paths (through ScavTrap and FragTrap).
	- This causes duplicate instances of the parent class in the child class (DiamondTrap).

	Solution:
	- Use the `virtual` keyword in ScavTrap and FragTrap when inheriting from ClapTrap.
	- This ensures that ClapTrap is inherited as a single shared instance,
	  avoiding duplication and potential conflicts.
*/

class DiamondTrap : public ScavTrap, public FragTrap {
	private:
		std::string	_name;

	public:
		DiamondTrap();
		DiamondTrap(std::string name);
		DiamondTrap(const DiamondTrap &rightSide);
		DiamondTrap	&operator=(const DiamondTrap &rightSide);
		~DiamondTrap();

		using	ClapTrap::operator=;
		using	ScavTrap::attack;
		void	whoAmI(void);
};

#endif
