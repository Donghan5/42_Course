/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serialize.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 22:57:31 by donghank          #+#    #+#             */
/*   Updated: 2025/01/05 23:12:29 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP
#pragma once

#include <stdint.h>
#include "Data.hpp"

class Serializer {
	public:
		Serializer();
		Serializer(const Serializer &rhs);
		~Serializer();

		Serializer &operator=(const Serializer &rhs);
		uintptr_t	serialize(Data* ptr);
		Data* deserialize(uintptr_t raw);
};

#endif
