/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MutantStack.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 22:18:13 by donghank          #+#    #+#             */
/*   Updated: 2025/01/14 11:14:51 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MUTANTSTACK_HPP
#define MUTANTSTACK_HPP
#pragma once

#include <iostream>
#include <algorithm>
#include <stack>
#include <list>
#include <deque>

/*
	template class T --> type
			 class Container = deque<T> --> default type = deque
*/
template <class T>
class MutantStack: public std::stack< T > {
	private:

	public:
		MutantStack() {};
		MutantStack(const MutantStack &rhs) { *this = rhs; };
		~MutantStack() {};
		MutantStack &operator=(const MutantStack &rhs) {
			if (this != &rhs) {
				std::stack< T >::operator=(rhs);
			}
			return *this;
		};

		typedef typename std::stack<T>::container_type::iterator	iterator;
		typedef typename std::stack<T>::container_type::const_iterator const_iterator;
		typedef typename std::stack<T>::container_type::reverse_iterator reverse_iterator;
		typedef typename std::stack<T>::container_type::const_reverse_iterator const_reverse_iterator;

		// iterator part
		iterator	begin() { return this->c.begin(); };
		iterator	end() { return this->c.end(); };

		// const_iterator part
		const_iterator	begin() const { return this->c.begin(); };
		const_iterator	end() const { return this->c.end(); };

		// reverse_iterator
		reverse_iterator	rbegin() { return this->c.rbegin(); };
		reverse_iterator	rend() { return this->c.rend(); };

		// const_reverse_iterator
		const_reverse_iterator	rbegin() const { return this->c.rbegin(); };
		const_reverse_iterator	rend() const { return this->c.rend(); };
};

#endif
