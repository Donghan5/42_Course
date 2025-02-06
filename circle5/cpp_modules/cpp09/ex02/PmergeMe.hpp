/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:16:04 by donghank          #+#    #+#             */
/*   Updated: 2025/02/05 11:44:56 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP
#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>


/*
	Using vector and list for data storage:
	- vector: Efficient for handling large datasets and random access.
	- list: Optimized for frequent insertions and deletions.

	Algorithm explanation:
		1. Convert string input into integers.
		2. Generate pairs of values, ensuring the larger value comes first.
		3. Use the Jacobsthal sequence to determine the optimal insertion positions.
		4. Insert values at their correct positions using binary search.
		5. Ensure the final sequence remains sorted.

	Reason Binary search + Jacobsthal sequence + merge-inserction:
		- The most efficient way to sort elements
		- The Jacobsthal sequence increases almost 2x, making it easy to locate positions using binary search
		- In merge-insertion, we first create pairs before sorting
*/

// type definitions
typedef std::vector<int> int_vector;

template <typename Container>
class PmergeMe {
	public:
		// type definitions --> flexibility
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef std::pair <value_type, value_type> type_pair;
		typedef std::vector <type_pair> pair_vector;
	private:
		Container _data;
		value_type _last;
		double _time;

		int_vector	generateIndexes(size_t size);
		pair_vector generatePairs(Container &data);
		void		sortPairs(pair_vector &pairs);
		int			binarySearch(int target);
		int			ftStoa(const char *str);
		bool		isNum(const std::string &s);

	public:
		PmergeMe();
		explicit PmergeMe(char **data); // not to change instantly
		PmergeMe(const PmergeMe &rhs);
		~PmergeMe();

		PmergeMe &operator=(const PmergeMe &rhs);


		// sorting algorithms
		void	sort();
		void	printData() const;
		void	timeTable() const;

		// Exception
		class InvalidInputException: public std::exception {
			public:
				const char *what() const throw();
		};
};

#endif
