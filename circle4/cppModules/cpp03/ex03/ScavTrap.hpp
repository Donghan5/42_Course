/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScavTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:20:11 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:20:12 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAVTRAP_HPP
#define SCAVTRAP_HPP
#pragma once

#include "ClapTrap.hpp"

/*
	inheritance --> protected, public --> inheritance
					private --> not inheritance
	Why we use the inheritance --> reusability of the code
*/
class ScavTrap : virtual public ClapTrap {

	public:
		ScavTrap(void);
		ScavTrap(std::string name);
		ScavTrap(const ScavTrap &rightSide);
		ScavTrap	&operator=(const ScavTrap &rightSide);
		~ScavTrap();

		void	attack(const std::string& target);
		void	guradGate(void);
};

#endif
