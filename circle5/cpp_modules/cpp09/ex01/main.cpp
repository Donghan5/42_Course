/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 23:48:52 by donghank          #+#    #+#             */
/*   Updated: 2025/01/19 14:39:27 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

#define RED "\033[31m"
#define END "\033[0m"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << RED << "Usage: ./RPN (RPN equatation)" << END << std::endl;
		return 1;
	}
	Rpn	rpn(argv[1]);
	rpn.printResultOfRpn();
	return 0;
}
