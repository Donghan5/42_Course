/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:39 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:14:40 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sed.hpp"

int	main(int argc, char **argv) {
	if (argc != 4) {
		std::cout << "Please respect the form. './ex04 filename s1 s2'" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		Sed	Sed(argv[1]);
		Sed.replace(argv[2], argv[3]);
	}
	return (0);
}
