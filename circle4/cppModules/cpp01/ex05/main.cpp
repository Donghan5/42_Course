/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:56 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:14:57 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"

int	main(void) {
	Harl		harl;

	std::cout << "\033[1;31mCALL DEBUG MODE\033[0m" << std::endl;
	harl.complain("DEBUG");
	std::cout << "\033[1;31mCALL INFO MODE\033[0m" << std::endl;
	harl.complain("INFO");
	std::cout << "\033[1;31mCALL WARNING MODE\033[0m" << std::endl;
	harl.complain("WARNING");
	std::cout << "\033[1;31mCALL ERROR MODE\033[0m" << std::endl;
	harl.complain("ERROR");
	return 0;
}
