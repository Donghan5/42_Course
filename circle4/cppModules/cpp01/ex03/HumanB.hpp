/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:16 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:14:17 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef HUMANB_HPP
#  define HUMANB_HPP

#pragma once
#include "Weapon.hpp"

/*
	by the condition of the subject, HumanB's weapon is choice
	he have or not. So Weapon to be pointer (to nullptr)
	in the setWeapon function, to avoid the nullptr, get the reference
*/
class HumanB {
	private:
		std::string	_name;
		Weapon*		_weapon;
		HumanB();

	public:
		HumanB(std::string _name);
		~HumanB();

	void	attack(void) const;
	void	setWeapon(Weapon &_weapon);

};
# endif
