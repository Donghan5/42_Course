/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:06:43 by kimdonghan        #+#    #+#             */
/*   Updated: 2025/01/22 11:59:45 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serialize.hpp"

int	main(void) {
	Data prev;
	Data *next;
	uintptr_t ptr;
	Serializer	se;

	prev.name = "Dong";
	prev.age = 42;
	std::cout << "Prev: " << prev.name;
	ptr = se.serialize(&prev);
	std::cout << " Serialize: " << ptr << std::endl;
	next = se.deserialize(ptr);
	std::cout << "Deserialize: " << next << std::endl;
	return 0;
}
