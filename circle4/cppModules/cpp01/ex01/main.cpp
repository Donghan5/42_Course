/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:13:25 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 21:08:46 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

// this the number of the Zombie
# define N 42

int	main(void) {
	Zombie	*zombiess = zombieHorde(N, "KING");

	for (int i = 0; i < N; i++) {
		zombiess[i].announce();
		std::cout << "Zombie number " << std::flush; // to verify the index of the zombie horde
		std::cout << i << " is called" << std::endl; // to verify the index of the zombie horde
	}
	delete[] zombiess;
	return (0);
}
