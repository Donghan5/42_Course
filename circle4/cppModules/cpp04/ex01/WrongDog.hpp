/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongDog.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:22:48 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:22:49 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRONGDOG_HPP
#define WRONGDOG_HPP
#pragma once

#include "WrongAnimal.hpp"

class WrongDog: public WrongAnimal {
	public:
		WrongDog();
		WrongDog(const WrongDog &rightSide);
		~WrongDog();

		using	WrongAnimal::operator=;
		void	makeSound(void) const;
};

#endif
