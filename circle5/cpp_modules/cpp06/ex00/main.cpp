/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 21:34:39 by donghank          #+#    #+#             */
/*   Updated: 2025/01/21 13:26:03 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Convert.hpp"

int	main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "usage: ./ex00 value" << std::endl;
		return 1;
	}
	ScalarConverter	c(argv[1]);
	try {
		c.convert();
		std::cout << c << std::endl;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
