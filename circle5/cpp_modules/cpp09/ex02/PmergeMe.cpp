/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:16:01 by donghank          #+#    #+#             */
/*   Updated: 2025/02/05 22:48:59 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

#define RED "\033[31m"
#define END "\033[0m"

template <typename Container>
const char *PmergeMe<Container>::InvalidInputException::what() const throw() { //exception
	return "Error";
}

template <typename Container>
std::string getContainerName();

template <>
std::string getContainerName<std::vector<int> >() { // getter
	return "vector<int>";
}

template <>
std::string getContainerName<std::list<int> >() { // getter
	return "list<int>";

}

template<typename Container>
PmergeMe<Container>::PmergeMe() : _data(), _last(-1), _time(0) {} // default constructor

template <typename Container>
PmergeMe<Container>::PmergeMe(char **data): _data() { // type constructor
	_time = 0;
	_last = 1;

	for (size_t i (0); data[i]; i++) { // to verify the element is number
		if (!isNum(data[i])) {
			throw PmergeMe::InvalidInputException();
		}
		_data.push_back(ftStoa(data[i]));
	}
}

template <typename Container>
PmergeMe<Container>::PmergeMe(const PmergeMe &rhs) { // copy constructor
	this->_data = rhs._data;
}

template <typename Container>
PmergeMe<Container> &PmergeMe<Container>::operator=(const PmergeMe &rhs) { // copy assignement
	if (this != &rhs) {
		this->_data = rhs._data;
		this->_last = rhs._last;
		this->_time = rhs._time;
	}
	return *this;
}

template <typename Container>
PmergeMe<Container>::~PmergeMe() {} // destructor

/*
	convert a string to an integer
	@param
		str: numeric string to be converted
	@return
		converted integer value
*/
template <typename Container>
int PmergeMe<Container>::ftStoa(const char *str) {
	std::istringstream ss(str);
	int number;
	ss >> number; // Convert the string
	return (number);
}

/*
	verify if a given string is a numeric
	@param
		s: string to verify
	@return
		true: if string is numeric
		false: if string isn't numeric
*/
template <typename Container>
bool PmergeMe<Container>::isNum(const std::string &s) {
	if (s.empty() || s[0] == '-') return false; // check negative integer case
	size_t i = 0;
	if (s[0] == '+' && s.length() == 1) return false; // check "+" case (just + in str)
	else if (s[0] == '+') i++; // if meet +, increment
	for (; i < s.length(); i++) {
		if (!std::isdigit(s[i])) {
			return false;
		}
	}
	return true;
}

/*
	Generate pairs of numbers to prepare for the merge-insertion algorithm.
	@param
		data: Container holding the number to be paired
	@return
		A vector of paired value
*/
template <typename Container>
typename PmergeMe<Container>::pair_vector PmergeMe<Container>::generatePairs(Container &data) {
	pair_vector pairs;

	if (data.size() % 2 != 0) { // If the amount of data is odd, add one more data
		this->_last = data.back();
		data.pop_back();
	} else _last = -1;

	typename Container::iterator it = data.begin();
	while (it != data.end()) {
		typename Container::iterator first = it;
		typename Container::iterator second = first;
		++second;

		if (second == data.end()) break;

		if (*first < *second) std::swap(*first, *second);

		pairs.push_back(std::make_pair(*first, *second));
		std::advance(it, 2);
	}
	return pairs;
}

/*
    Sort all pairs using a recursive merge sort approach.
    @param
        pairs: A vector of number pairs generated by generatePairs().
*/
template <typename Container>
void PmergeMe<Container>::sortPairs(pair_vector &pairs) {
	if (pairs.size() <= 1) return ; // nothing to sort

	size_type middle = pairs.size() / 2;
	pair_vector leftHalf(pairs.begin(), pairs.begin() + middle);
	pair_vector rightHalf(pairs.begin() + middle, pairs.end());

	// sorting the element
	sortPairs(leftHalf);
	sortPairs(rightHalf);

	size_type leftIdx = 0;
	size_type rightIdx = 0;
	size_type idx = 0;

	// increment left or right index
	while (leftIdx < leftHalf.size() && rightIdx < rightHalf.size()) {
		pairs[idx++] = leftHalf[leftIdx].first < rightHalf[rightIdx].first ? leftHalf[leftIdx++] : rightHalf[rightIdx++];
	}

	// increment total index
	while (leftIdx < leftHalf.size()) {
		pairs[idx++] = leftHalf[leftIdx++];
	}
	while (rightIdx < rightHalf.size()) {
		pairs[idx++] = rightHalf[rightIdx++];
	}
}

/*
	Perform a binary search to find the appropriate position for insertion.
	1. Divide the range into two halves.
	2. Recursively search the left half.
	3. Recursively search the right half.
	4. Repeat until the target position is found.

Example:
	- Imagine the list [0, 1, 2, 3, 5, 6]
	- The target value is 4
	- In this case, left will increase its index, while right will decrease its index
	- If the target is found in the middle, return it
	- Otherwise, return the left value
		- But why return left?
			- Because left keeps increasing as the search progresses
			- In this case: left becomes 5, right becomes 3. So, left always points to the correct insertion position
	@param
		target: The value to find.
	@return
		The index where the value should be inserted.
*/
template <typename Container>
int PmergeMe<Container>::binarySearch(int target) {
	int left = 0;
	int right = _data.size() - 1;

	while (left <= right) {
		int mid = (left + right) / 2;

		typename Container::iterator midIt = _data.begin();
		std::advance(midIt, mid);

		if (*midIt == target) return mid;
		else if (*midIt < target) left = mid + 1;
		else right = mid - 1;
	}

	return left;
}

/*
	Jacobsthal sequence explanation:
	- The sequence grows similarly to the Fibonacci sequence but with a slight difference.
	- Formula: J(n) = J(n - 1) + 2 * J(n - 2)
	- Initial values: J(0) = 0, J(1) = 1
	- Sequences: 0, 1, 1, 3, 5, ....

	- Helps in determining optimal insertion positions.
	- Used in merge-insertion sorting to efficiently insert values.
	the reason of the lastJacobthalNumber is 2:
		- initial values is 0 and 1, if we use 1, possibility same values and also bad sort
*/
template <typename Container>
int_vector PmergeMe<Container>::generateIndexes(size_t size) { // Using vector
	int_vector indexes;
	std::vector<int> jacobsthalSeq;

	jacobsthalSeq.push_back(0);
	jacobsthalSeq.push_back(1);

	int lastJacobsthalNumber = 2;
	for (size_t i = 2; indexes.size() < size; i++) {
		int nextJacobsthal = jacobsthalSeq[i - 1] + 2 * jacobsthalSeq[i - 2];
		jacobsthalSeq.push_back(nextJacobsthal);

		if (i > 2) indexes.push_back(nextJacobsthal);

		for (int j = nextJacobsthal - 1; j > lastJacobsthalNumber; j--) {
			indexes.push_back(j);
		}
		lastJacobsthalNumber = nextJacobsthal;
	}
	return indexes;
}

/*
	Using index version
*/
// template <typename Container>
// int_vector PmergeMe<Container>::generateIndexes(size_t size) {
// 	int_vector indexes;
// 	int jacobsthalSeq[size + 1];

// 	jacobsthalSeq[0] = 0;
// 	jacobsthalSeq[1] = 1;
// 	int lastJacobsthalNumber = 2; // store last element of jacobsthal sequence
// 	for (size_t i = 2; indexes.size() < size; i++) {
// 		jacobsthalSeq[i] = jacobsthalSeq[i - 1] + 2 * jacobsthalSeq[i - 2];

// 		// push the jacobsthal num to sequence
// 		i != 2 ? indexes.push_back(jacobsthalSeq[i]) : (void)0;

// 		for (int j = jacobsthalSeq[i] - 1; j > lastJacobsthalNumber; j--)
// 			indexes.push_back(j);

// 		lastJacobsthalNumber = jacobsthalSeq[i];
// 	}
// 	return indexes;
// }

/*
	Showing the datas
*/
template <typename Container>
void PmergeMe<Container>::printData() const {
	typename Container::const_iterator it = _data.begin();
	for (; it != _data.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

/*
	Showing passed time to sort
*/
template <typename Container>
void PmergeMe<Container>::timeTable() const {
	std::cout << "Time to process range of " << _data.size() << " element with std::" << getContainerName< Container >() << " : " << std::fixed << std::setprecision(5) << _time << " us" << std::endl;
}

/*
	Do sorting (all elements)
*/
template <typename Container>
void PmergeMe<Container>::sort() {
	clock_t start = clock();

	pair_vector pairs = generatePairs(_data);
	int_vector insertionIndexes = generateIndexes(_data.size());

	if (pairs.size() == 0) {
		_data.push_back(_last);
		_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
		return;
	}

	sortPairs(pairs); // sorting pairs

	_data.clear();

	_data.push_back(pairs[0].second);

	for (size_type i = 0; i < pairs.size(); i++) {
		int value = pairs[i].first;
		size_t index = binarySearch(value);
		typename Container::iterator insertIt = _data.begin();
		std::advance(insertIt, index); // increment per index
		_data.insert(insertIt, value);
	}

	for (size_t i = 0; i < insertionIndexes.size(); i++) {
		if (size_t(insertionIndexes[i] - 1) >= pairs.size()) continue ;

		int value = pairs[insertionIndexes[i] - 1].second;
		size_t index = binarySearch(value);

		// if the case of the list --> advance
		typename Container::iterator insertIt = _data.begin();
		std::advance(insertIt, index);

		_data.insert(insertIt, value);
	}

	if (_last != -1) {
		size_t index = binarySearch(_last);
		typename Container::iterator insertIt = _data.begin();
		std::advance(insertIt, index);

		_data.insert(insertIt, _last);
	}

	_time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
}

// Excplicit instantiation
template class PmergeMe<std::vector<int> >;
template class PmergeMe<std::list<int> >;
