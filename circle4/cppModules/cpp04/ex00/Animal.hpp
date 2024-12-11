/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:20:37 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:20:38 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMAL_HPP
#define ANIMAL_HPP
#pragma once

#include <iostream>
#include <string>

class Animal {
	protected:
		std::string	type;

	public:
		Animal();
		// const --> use read-only
		Animal(const std::string &type);
		Animal(const Animal &rightSide);
		Animal	&operator=(const Animal &rightSide);
		virtual ~Animal();

		virtual void		makeSound(void) const;
		const std::string	&getType(void) const;
		// have to mod the member
		void				setType(const std::string &type);
};

#endif
