/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:40:04 by donghank          #+#    #+#             */
/*   Updated: 2025/01/29 14:29:24 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Span.hpp"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define END "\033[0m"

int main(void) {
	std::cout << GREEN << "===============================================" << END << std::endl;
	std::cout << GREEN << "|              NORMAL CASE TESTS              |" << END << std::endl;
	std::cout << GREEN << "===============================================" << END << std::endl;
	{
		std::cout << GREEN << "=========== Subject test ===========" << END << std::endl;
		Span sp = Span(5);
		try {
			sp.addNumber(6);
			sp.addNumber(3);
			sp.addNumber(17);
			sp.addNumber(9);
			sp.addNumber(11);
			std::cout << sp.shortestSpan() << std::endl;
			std::cout << sp.longestSpan() << std::endl;
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}

	{
		std::cout << GREEN << "=========== Custom test (a lot of number) ===========" << END << std::endl;
		Span sp = Span(10000);
		srand(time(NULL));
		try {
			for (int i (0); i < 10000; i++) {
				sp.addNumber(rand());
			}
			std::cout << sp.shortestSpan() << std::endl;
			std::cout << sp.longestSpan() << std::endl;
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
		std::cout << RED << "[INFO]" << END << "It can provoque the already exist error: using random number" << std::endl;
	}

	{
		std::cout << GREEN << "========== Custom test (negative number) ==========" << END << std::endl;
		Span sp = Span(3);
		try {
			sp.addNumber(-50);
			sp.addNumber(-10);
			sp.addNumber(32);
			std::cout << sp.shortestSpan() << std::endl;
			std::cout << sp.longestSpan() << std::endl;
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}
	std::cout << RED << "===============================================" << END << std::endl;
	std::cout << RED << "|            ERROR INTEDED TESTS              |" << END << std::endl;
	std::cout << RED << "===============================================" << END << std::endl;
	{
		std::cout << RED << "=========== Custom test (same number) ===========" << END << std::endl;
		Span sp = Span(5);
		try {
				sp.addNumber(42);
				sp.addNumber(42);
				sp.addNumber(42);
				sp.addNumber(42);
				sp.addNumber(42);
				std::cout << sp.shortestSpan() << std::endl;
				std::cout << sp.longestSpan() << std::endl;
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}

	{
		std::cout << RED << "=========== Custom test (No more spaces) ===========" << END << std::endl;
		Span sp = Span(5);
		try {
			sp.addNumber(6);
			sp.addNumber(3);
			sp.addNumber(17);
			sp.addNumber(9);
			sp.addNumber(11);
			sp.addNumber(10);
			std::cout << sp.shortestSpan() << std::endl;
			std::cout << sp.longestSpan() << std::endl;
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}

	{
		std::cout << RED << "=========== Custom test (same number ver 2) ===========" << END << std::endl;
		Span sp = Span(5);
		try {
			sp.addNumber(6);
			sp.addNumber(6);
			sp.addNumber(17);
			sp.addNumber(9);
			sp.addNumber(11);
			std::cout << sp.shortestSpan() << std::endl;
			std::cout << sp.longestSpan() << std::endl;
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}

	{
		std::cout << RED << "=========== Custom test (few data) ===========" << END << std::endl;
		Span sp = Span(1);
		try {
			sp.addNumber(42);
			std::cout << sp.shortestSpan() << std::endl;
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}

	return 0;
}
