/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:10 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:14:11 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef HUMANA_HPP
#  define HUMANA_HPP
# pragma once

#include "Weapon.hpp"

class HumanA {
	private:
		std::string	_name;
		Weapon&		_weapon;
		HumanA();

	public:
		HumanA(std::string _name, Weapon &_weapon);
		~HumanA();

	void	attack(void) const;

};

# endif
