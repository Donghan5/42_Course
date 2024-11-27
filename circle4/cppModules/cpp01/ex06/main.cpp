/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:15:12 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:15:13 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"

int	main(int argc, char **argv) {
	Harl	Harl;

	if (argc != 2) {
		std::cout << "Make sure amount of argument" << std::endl;
		return (1);
	}
	Harl.complain(argv[1]);
	return (0);
}
