/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:22:21 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:22:22 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAT_HPP
#define CAT_HPP
#pragma once

#include "Animal.hpp"
#include "Brain.hpp"

class Cat: public Animal {
	private:
		Brain	*_brain;
	public:
		Cat();
		Cat(const Cat &rightSide);
		~Cat();

		Animal	&operator=(const Animal &rightSide);
		void	makeSound(void) const;
		Brain	*getBrain(void) const;
};

#endif
