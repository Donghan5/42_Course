/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:24:20 by donghank          #+#    #+#             */
/*   Updated: 2024/12/10 12:47:02 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ICharacter.hpp"
#include "IMateriaSource.hpp"
#include "AMateria.hpp"
#include "MateriaSource.hpp"
#include "Ice.hpp"
#include "Cure.hpp"
#include "Character.hpp"

/*
	Interface in C++
		Change the status of tight coupling(between of the objs) to loose coupling
		Like a pipe, imagine there's a lot of code in project, by using interface of C++
		it's easy to maintain and build the project
*/
int main()
{
	IMateriaSource* src = new MateriaSource();
	src->learnMateria(new Ice());
	src->learnMateria(new Cure());
	ICharacter* me = new Character("me");
	AMateria* ice = src->createMateria("ice");
	me->equip(ice);
	AMateria* cure = src->createMateria("cure");
	me->equip(cure);
	ICharacter* bob = new Character("bob");
	me->use(0, *bob);
	me->use(1, *bob);
	me->unequip(0);
	me->unequip(1);
	// std::cout << "------- This is to check unequipe is worked well should not delete cure and ice -------" << std::endl;
	// std::cout << "Cure left: " << cure->getType() << "; Ice left: " << ice->getType() << std::endl;
	delete ice;
	delete cure;
	delete bob;
	delete me;
	delete src;
	return 0;
}
