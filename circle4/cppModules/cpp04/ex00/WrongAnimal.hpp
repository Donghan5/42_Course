/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:21:10 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:21:11 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRONGANIMAL_HPP
#define WRONGANIMAL_HPP
#pragma once

#include <iostream>
#include <string>

class WrongAnimal {
	protected:
		std::string	type;
	public:
		WrongAnimal();
		WrongAnimal(const std::string &type);
		WrongAnimal(const WrongAnimal &rightSide);
		WrongAnimal	&operator=(const WrongAnimal &rightSide);
		~WrongAnimal();

		void				makeSound(void) const;
		const std::string	&getType(void) const;
		void				setType(const std::string &type);
};

#endif
