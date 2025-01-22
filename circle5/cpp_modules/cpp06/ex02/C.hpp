/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C.hpp                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:03:16 by donghank          #+#    #+#             */
/*   Updated: 2025/01/06 21:04:35 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_HPP
#define C_HPP
#pragma once

#include "Base.hpp"

class C: public Base {
	public:
		C();
		C(const C &rhs);
		~C();

		C	&operator=(const C&);
};

#endif
