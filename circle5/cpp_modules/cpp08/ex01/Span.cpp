/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 23:58:44 by donghank          #+#    #+#             */
/*   Updated: 2025/01/29 13:36:38 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Span.hpp"

/*
	Exceptions
*/
const char	*Span::AlreadyExistException::what() const throw() {
	return "Element already exist";
}

const char	*Span::NoMoreSpaceException::what() const throw() {
	return "No more space";
}

const char	*Span::NotEnoughSpaceException::what() const throw() {
	return "Number of elements at least 2";
}

Span::Span(): _N(0), storage(0, 0) { this->storage.reserve(0); } // Default constructor

/*
	Constructor
*/
Span::Span(unsigned int N): _N(N) {
	this->storage.reserve(_N);
}

/*
	Copy assignement
*/
Span::Span(const Span &rhs) {
	if (this != &rhs) {
		this->_N = rhs._N;
		this->storage = rhs.storage;
	}
}

Span::~Span() {}; // Destructor

/*
	Get the size of Span
	@return
		size of the span
*/
int	Span::sizeSpan() const {
	return this->storage.size();
}

/*
	Get the capacity
	@return
		N: unsigned int
*/
int	Span::capacitySpan() const {
	return this->_N;
}

/*
	Adding numbers, if the size is over than capacity throw an error
	And already exists throw an error
*/
void	Span::addNumber(int addNum) {
	if (this->storage.size() >= _N) {
		throw Span::NoMoreSpaceException();
	}
	std::vector<int>::iterator it;
	for (it = storage.begin(); it != storage.end(); ++it) {
		if (*it == addNum) {
			throw Span::AlreadyExistException();
		}
	}
	storage.push_back(addNum);
}

int	Span::shortestSpan(void) const {
	std::vector<int> tmp;

	tmp = storage;
	if (this->storage.size() < 2) {
		throw Span::NotEnoughSpaceException();
	}
	std::sort(tmp.begin(), tmp.end());
	int shortestGap = tmp.at(1) - tmp.at(0);

	std::vector<int>::iterator it;
	for (it = tmp.begin(); it != tmp.end() - 1; it++) {
		if (abs(*(it + 1) - *it) < shortestGap) {
			shortestGap = abs(*(it + 1) - *it);
		}
	}
	return shortestGap;
}

int	Span::longestSpan(void) const {
	std::vector<int> tmp;

	tmp = storage;
	if (this->storage.size() < 2) {
		throw Span::NotEnoughSpaceException();
	}
	std::sort(tmp.begin(), tmp.end());
	int min = *std::min_element(tmp.begin(), tmp.end());
	int max = *std::max_element(tmp.begin(), tmp.end());
	return abs(max - min);
}
