/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:07:01 by donghank          #+#    #+#             */
/*   Updated: 2025/01/29 14:39:11 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easyfind.hpp"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define END "\033[0m"

int main(void) {
	{
		std::cout << GREEN << "\n=================================================================" << END << std::endl;
		std::cout << GREEN << "|      NORMAL CASE TEST: all test must to work properly!!!      |" << END << std::endl;
		std::cout << GREEN << "=================================================================" << END << std::endl;
		std::set<int> set;
		std::deque<int> deque;
		std::list<int> list;
		std::vector<int> vector;

		std::cout << "=========== INT TEST ===========" << std::endl;
		std::cout << "Assignement....." << std::endl;
		for (int i (0); i < 10; i++) {
			set.insert(i);
			deque.push_front(i);
			list.push_back(i);
			vector.push_back(i);
		}
		try {
			std::cout << "------ Set test ------" << std::endl;
			for (std::set<int>::iterator it = set.begin(); it != set.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::set<int>::iterator set_it = easyfind(set, 0);
			std::cout << GREEN << "\nFound: " << *set_it << " followed by " << *(++set_it) << END << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "----- deque test -----" << std::endl;
			for (std::deque<int>::iterator it = deque.begin(); it != deque.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::deque<int>::iterator deque_it = easyfind(deque, 1);
			std::cout << GREEN << "\nFound: " << *deque_it << " followed by " << *(++deque_it) << END << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "----- list test ------" << std::endl;
			for (std::list<int>::iterator it = list.begin(); it != list.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::list<int>::iterator list_it = easyfind(list, 2);
			std::cout << GREEN << "\nFound: " << *list_it << " followed by " << *(++list_it) << GREEN << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "------ vector test ------" << std::endl;
			for (std::vector<int>::iterator it = vector.begin(); it != vector.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find....." << END << std::endl;
			std::vector<int>::iterator vector_it = easyfind(vector, 3);
			std::cout << GREEN <<"\nFound: " << *vector_it << " followed by " << *(++vector_it) << END << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
	}
	{
		std::set<double> set;
		std::deque<double> deque;
		std::list<double> list;
		std::vector<double> vector;

		std::cout << "=========== DOUBLE TEST ===========" << std::endl;
		std::cout << "Assignement....." << std::endl;
		for (double i (0); i < 10.0; i++) {
			set.insert(i);
			deque.push_front(i);
			list.push_back(i);
			vector.push_back(i);
		}
		try {
			std::cout << "------ Set test ------" << std::endl;
			for (std::set<double>::iterator it = set.begin(); it != set.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::set<double>::iterator set_it = easyfind(set, 0.0);
			std::cout << GREEN << "\nFound: " << *set_it << " followed by " << *(++set_it) << END << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "----- deque test -----" << std::endl;
			for (std::deque<double>::iterator it = deque.begin(); it != deque.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::deque<double>::iterator deque_it = easyfind(deque, 1.0);
			std::cout << GREEN << "\nFound: " << *deque_it << " followed by " << *(++deque_it) << END << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "----- list test ------" << std::endl;
			for (std::list<double>::iterator it = list.begin(); it != list.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::list<double>::iterator list_it = easyfind(list, 2.0);
			std::cout << GREEN << "\nFound: " << *list_it << " followed by " << *(++list_it) << END << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "------ vector test ------" << std::endl;
			for (std::vector<double>::iterator it = vector.begin(); it != vector.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find....." << END << std::endl;
			std::vector<double>::iterator vector_it = easyfind(vector, 3.0);
			std::cout << GREEN << "\nFound: " << *vector_it << " followed by " << *(++vector_it) << END << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
	}
	{
		std::set<int> set;
		std::deque<int> deque;
		std::list<int> list;
		std::vector<int> vector;

		std::cout << RED << "\n=================================================================" << END << std::endl;
		std::cout << RED << "|  INTENDED ERROR TEST: all test must to be return exception!!  |" << END << std::endl;
		std::cout << RED << "=================================================================" << END << std::endl;
		std::cout << "=========== INT TEST ===========" << std::endl;
		std::cout << "Assignement....." << std::endl;
		for (int i (0); i < 10; i++) {
			set.insert(i);
			deque.push_front(i);
			list.push_back(i);
			vector.push_back(i);
		}
		try {
			std::cout << "------ Set test ------" << std::endl;
			for (std::set<int>::iterator it = set.begin(); it != set.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::set<int>::iterator set_it = easyfind(set, 30);
			std::cout << "\nFound: " << *set_it << " followed by " << *(set_it++) << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "----- deque test -----" << std::endl;
			for (std::deque<int>::iterator it = deque.begin(); it != deque.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::deque<int>::iterator deque_it = easyfind(deque, 33);
			std::cout << "\nFound: " << *deque_it << " followed by " << *(deque_it++) << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "----- list test ------" << std::endl;
			for (std::list<int>::iterator it = list.begin(); it != list.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find...." << END << std::endl;
			std::list<int>::iterator list_it = easyfind(list, 42);
			std::cout << "\nFound: " << *list_it << " followed by " << *(list_it++) << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
		try {
			std::cout << "------ vector test ------" << std::endl;
			for (std::vector<int>::iterator it = vector.begin(); it != vector.end(); it++) {
				std::cout << ' ' << *it;
			}
			std::cout << RED << "\nTry to find....." << END << std::endl;
			std::vector<int>::iterator vector_it = easyfind(vector, 42);
			std::cout << "\nFound: " << *vector_it << " followed by " << *(vector_it++) << std::endl;
		} catch (const std::exception &e) {
			std::cerr << "[Exception] " << e.what() << std::endl;
		}
	}
	return 0;
}
