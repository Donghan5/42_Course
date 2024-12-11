/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MateriaSource.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:24:25 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:24:26 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MateriaSource.hpp"

MateriaSource::MateriaSource() {
	// std::cout << "Default constructor of MateriaSource has been called." << std::endl
	for (int i = 0; i < 4; i++)
		Materias[i] = NULL;
}

MateriaSource::MateriaSource(const MateriaSource &rightSide) {
	// std::cout << "Copy-constructor of the MateriaSource has been called." << std::endl;
	*this = rightSide;
}

MateriaSource::~MateriaSource() {
	// std::cout << "Destructor of the MateriaSource has been called." << std::endl;
	for (int i = 0; i < 4; i++)
		if (Materias[i])
			delete Materias[i];
}

MateriaSource	&MateriaSource::operator=(const MateriaSource &rightSide) {
	if (this != &rightSide) {
		for (int i = 0; i < 4; i++) {
			this->Materias[i] = rightSide.Materias[i];
		}
	}
	return (*this);
}

void	MateriaSource::learnMateria(AMateria *m) {
	for (int i = 0; i < 4; i++) {
		if (Materias[i] == NULL) {
			Materias[i] = m;
			// std::cout << "The element of Materias well learned at " << m->getType() << std::endl;
			return ;
		}
	}
	// std::cout << "The element of Materias can't learned at " << m->getType() << std::endl;
}

AMateria	*MateriaSource::getMateria(std::string const &type) {
	for (int i = 0; i < 4; i++) {
		if (Materias[i] && Materias[i]->getType() == type)
			return (Materias[i]);
	}
	return (NULL);
}

AMateria	*MateriaSource::createMateria(std::string const &type) {
	for (int i = 0; i < 4; i++) {
		if (Materias[i] && Materias[i]->getType() == type)
			return (Materias[i]->clone());
	}
	return (NULL);
}
