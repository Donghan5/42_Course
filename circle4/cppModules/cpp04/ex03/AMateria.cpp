/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMateria.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:47 by donghank          #+#    #+#             */
/*   Updated: 2024/12/06 20:50:05 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMateria.hpp"

/*
	Constructor
	@param
		type: type of the AMateria
*/
AMateria::AMateria(std::string const &type): _type(type) {
	// std::cout << "Type constructor of AMateria has been called" << std::endl;
}

/*
	Copy constructor
	@param
		rightSide: copy obj
*/
AMateria::AMateria(const AMateria &rightSide) {
	// std::cout << "Copy constructor of AMateria has been called" << std::endl;
	*this = rightSide;
}

/*
	Destructor
*/
AMateria::~AMateria() {
	// std::cout << "Destructor of AMateria has been called" << std::endl;
}

/*
	Copy assingnement
	@param
		rightSide: copy obj
	@return
		this = self-pointer of the class
*/
AMateria	&AMateria::operator=(const AMateria &rightSide) {
	this->_type = rightSide._type;
	return (*this);
}

/*
	getter
	@return
		this->_type: type of the materia
*/
std::string const	&AMateria::getType(void) const {
	return (this->_type);
}

/*
	setter
*/
void	AMateria::setType(std::string type) {
	this->_type = type;
}
