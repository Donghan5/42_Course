/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 14:10:50 by donghank          #+#    #+#             */
/*   Updated: 2025/01/23 13:21:29 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Array.hpp"

#define MAX_VAL 100
#define RED "\033[31m"
#define END "\033[0m"
#define MAX_IDX 5

int main()
{
	std::cout << RED << "========== START TEST ==========" << END << std::endl;

	srand(time(NULL));
	Array<int> numbers(MAX_VAL);
	std::cout << "Assignement numbers....." << std::endl;
	for (int i (0); i < MAX_VAL; i++) {
		numbers[i] = rand() % 100;
	}

	std::cout << "========== print numbers =========" << std::endl;
	for (int i (0); i < MAX_VAL; i++) {
		std::cout << numbers[i] << std::endl;
	}

	Array<char> chars(MAX_IDX);
	std::cout << "====================================" << std::endl;
	std::cout << "Assignemnt chars....." << std::endl;
	chars[0] = 'a';
	chars[1] = 'b';
	chars[2] = 'c';
	chars[3] = 'd';
	chars[4] = 'e';

	std::cout << "========== print chars ==========" << std::endl;
	for (int i(0); i < MAX_IDX; i++) {
		std::cout << chars[i] << std::endl;
	}

	std::cout << "========== Exception test (Intented error) ==========" << std::endl;
	std::cout << RED << "Expected output: [Exception] Index is out of bound" << END << std::endl;
	Array<int> numberException(5);
	std::cout << "====================================" << std::endl;
	std::cout << "Assignement numbers...." << std::endl;
	for (int i (0); i < 5; i++) {
		numberException[i] = rand();
	}

	std::cout << "========== print numbers ==========" << std::endl;
	try {
		for (int i (-1); i < 5; i++) {
			std::cout << numberException[i] << std::endl;
		}
	} catch (std::exception &e) {
		std::cerr << "[Exception] " << e.what() << std::endl;
	}
	std::cout << RED << "========== END TEST ==========" << END << std::endl;
	return 0;
}
