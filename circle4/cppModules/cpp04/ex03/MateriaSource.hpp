/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:24:28 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:24:29 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIASOURCE_HPP
#define MATERIASOURCE_HPP
#pragma once

#include "IMateriaSource.hpp"
#include "AMateria.hpp"

class MateriaSource: public IMateriaSource {
	protected:
		AMateria	*Materias[4];

	public:
		MateriaSource();
		MateriaSource(const MateriaSource &rightSide);
		~MateriaSource();

		MateriaSource		&operator=(const MateriaSource &rightSide);
		void				learnMateria(AMateria *m);
		AMateria			*createMateria(std::string const &type);
		AMateria			*getMateria(std::string const &type);
};

#endif
