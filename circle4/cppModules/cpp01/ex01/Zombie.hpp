/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:13:34 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:13:35 by donghank         ###   ########.fr       */
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
		Zombie(void);
		~Zombie(void);
		void	announce(void);
		void	makeZombie(std::string name);
};

Zombie* zombieHorde(int N, std::string name);

# endif
