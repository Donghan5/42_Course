/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:38:03 by donghank          #+#    #+#             */
/*   Updated: 2025/01/16 12:43:18 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#define RED "\033[31m"
#define END "\033[0m"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << RED << "Usage: ./btc input.txt" << END << std::endl;
		return 1;
	}

	Btc	b;
	try {
		b.readInput(argv[1]);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
