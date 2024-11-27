/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newZombie.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:13:10 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 21:07:23 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

/*
	creating the new Zombie obj by using the new character
	new --> dynamical allocation of the memory
	after using this must have to use delete key-word
*/
Zombie	*newZombie(std::string name) {
	Zombie	*zptr = new Zombie(name);
	return (zptr);
}
