/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ice.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:24:10 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:24:11 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICE_HPP
#define ICE_HPP
#pragma once

#include "AMateria.hpp"
#include "ICharacter.hpp"

class Ice: public AMateria {
	public:
		Ice();
		Ice(const Ice &rightSide);
		~Ice();

		const Ice	&operator=(const Ice &rightSide);
		virtual AMateria	*clone(void) const;
		virtual void		use(ICharacter &target);
};

#endif
