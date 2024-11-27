/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:13:21 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:13:22 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ZOMBIE_HPP
#  define ZOMBIE_HPP

# pragma once
/* standard header file */
#include <iostream>

/* define class */
class Zombie {
	private:
		std::string _name;

	public:
		Zombie(std::string name);
		~Zombie();

		void	announce(void);
		Zombie	*newZombie(std::string name);
		void	ramdomChump(std::string name);
};

Zombie	*newZombie(std::string name);
void	randomChump(std::string name);

# endif
