/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMateria.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:50 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:23:51 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETERIA_HPP
#define AMETERIA_HPP
#pragma once

#include <iostream>
#include <string>
#include "ICharacter.hpp"
#include "IMateriaSource.hpp"

class ICharacter;

class AMateria {
	protected:
		std::string	_type;

	public:
		// AMateria();
		AMateria(std::string const & type);
		AMateria(const AMateria &rightSide);
		virtual ~AMateria();

		AMateria			&operator=(const AMateria &rightSide);
		std::string const&	getType() const; //Returns the materia type
		void				setType(std::string type);
		virtual AMateria*	clone() const = 0;
		virtual void 		use(ICharacter& target) = 0;
};
#endif
