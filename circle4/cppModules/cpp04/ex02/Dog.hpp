/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:15 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:23:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOG_HPP
#define DOG_HPP
#pragma once

#include "Animal.hpp"
#include "Brain.hpp"

class Dog: public Animal {
	private:
		Brain	*_brain;
	public:
		Dog();
		Dog(const Dog &rightSide);
		~Dog();

		Animal	&operator=(const Animal &rightSide);
		void	makeSound(void) const;
		Brain	*getBrain(void) const;
};

#endif
