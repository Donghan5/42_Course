/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A.hpp                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 20:43:25 by donghank          #+#    #+#             */
/*   Updated: 2025/01/06 23:51:57 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_HPP
#define A_HPP
#pragma once

#include "Base.hpp"

class A: public Base {
	public:
		A();
		A(const A &rhs);
		~A();

		A	&operator=(const A&);
};

#endif
