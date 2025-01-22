/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B.hpp                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:00:19 by donghank          #+#    #+#             */
/*   Updated: 2025/01/06 23:53:38 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef B_HPP
#define B_HPP
#pragma once

#include "Base.hpp"

class B: public Base {
	public:
		B();
		B(const B &rhs);
		~B();

		B	&operator=(const B&);
};

#endif
