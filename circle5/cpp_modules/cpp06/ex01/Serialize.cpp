/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serialize.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 22:57:13 by donghank          #+#    #+#             */
/*   Updated: 2025/01/21 12:41:48 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serialize.hpp"

Serializer::Serializer() {
	// std::cout << "Constructor has been called" << std::endl;
}

Serializer::Serializer(const Serializer &rhs) {
	*this = rhs;
}

Serializer::~Serializer() {
	// std::cout << "Destructor has been called" << std::endl;
}

Serializer	&Serializer::operator=(const Serializer &rhs) {
	(void) rhs;
	return *this;
}

/*
	reinterpret_cast
		change pointer type
		change between int and pointer
		reinterpret the bit
*/
uintptr_t	Serializer::serialize(Data* ptr) {
	return reinterpret_cast<uintptr_t>(ptr);
}

Data* Serializer::deserialize(uintptr_t raw) {
	return reinterpret_cast<Data*>(raw);
}
