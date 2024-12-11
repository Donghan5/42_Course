/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:20:58 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:20:59 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOG_HPP
#define DOG_HPP
#pragma once

#include "Animal.hpp"

class Dog: public Animal {
	public:
		Dog();
		Dog(const Dog &rightSide);
		~Dog();

		using	Animal::operator=;
		void	makeSound(void) const;
};

#endif
