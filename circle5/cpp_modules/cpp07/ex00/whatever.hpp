/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whatever.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:31:42 by donghank          #+#    #+#             */
/*   Updated: 2025/01/23 13:03:35 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHATEVER_HPP
#define WHATEVER_HPP
#pragma once

#include <iostream>

/*
	introduction of template
	goal: reutilization of the code
	typename T --> the type temporary, it decide the type when it complie
*/
template < typename T >
void	swap(T &a, T &b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template < typename T >
T	min(T &a, T &b) {
	return a < b ? a : b;
}

template < typename T >
T	max(T &a, T &b) {
	return a > b ? a : b;
}

#endif
