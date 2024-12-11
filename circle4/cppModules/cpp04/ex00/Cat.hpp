/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:20:50 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:20:51 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAT_HPP
#define CAT_HPP
#pragma once

#include "Animal.hpp"

class Cat: public Animal {
	public:
		Cat();
		Cat(const Cat &rightSide);
		~Cat();

		using	Animal::operator=;
		void	makeSound(void) const;
};

#endif
