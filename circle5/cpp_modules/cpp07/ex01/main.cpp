/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:52:11 by donghank          #+#    #+#             */
/*   Updated: 2025/01/22 20:33:33 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iter.hpp"

#define N 5

#define RED "\033[31m"
#define END "\033[0m"

int main(void) {
	int arr1[] = {1, 2, 3, 4, 5};
	std::cout << RED << "========== arr1 type int test ==========" << END << std::endl;
	iter(arr1, N, &printA);

	double arr2[] = {1.1, 2.2, 3.3, 4.4, 5.5};
	std::cout << RED << "========== arr2 type double test ==========" << END << std::endl;
	iter(arr2, N, &printA);

	float arr3[] = {1.11f, 2.22f, 3.33f, 4.44f, 5.55f};
	std::cout << RED << "========== arr3 type float test ==========" << END << std::endl;
	iter(arr3, N, &printA);

	char arr4[] = {'a', 'b', 'c', 'd', 'e'};
	std::cout << RED << "========== arr4 type char test ==========" << END<< std::endl;
	iter(arr4, N, &printA);

	int arr5[] = {1, 42, 45, 12, 5};
	std::cout << RED << "========== arr5 type int test ==========" << END << std::endl;
	iter(arr5, N, &printA);
}
