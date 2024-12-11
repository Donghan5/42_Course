/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cure.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:24:04 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:24:05 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CURE_HPP
#define CURE_HPP
#pragma once

#include "AMateria.hpp"
#include "ICharacter.hpp"

class Cure: public AMateria {
	public:
		Cure();
		Cure(const Cure &rightSide);
		~Cure();

		Cure	&operator=(const Cure &rightSide);
		virtual AMateria	*clone(void) const;
		virtual void		use(ICharacter &target);
};

#endif
