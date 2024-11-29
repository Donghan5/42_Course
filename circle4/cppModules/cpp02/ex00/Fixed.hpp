/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:15:25 by donghank          #+#    #+#             */
/*   Updated: 2024/11/28 12:39:15 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP
#pragma once

// standard lib(stream)
#include <iostream>
#include <string>

/*
	define the class
	Fixed value to binary
	Three part	- sign
				- Exponent : by using **bias** to control of the area of num
				- Fraction or Mantissa : store decimal part
	Converting decimal to binary --> 1. convert integer part
									 2. decimal multiple by 2, convert integer part
									 3. repeat whole this action
*/
class Fixed {
	private:
		int					_fixedPointVal;
		static const int	_fractionalBits = 8; // 8bits to stock
	public:
		Fixed();
		Fixed(const Fixed &rightSide); // copy constructor
		Fixed &operator = (const Fixed &rightSide); // A copy assignment operator overload.
		~Fixed();
		int		getRawBits(void) const;
		void	setRawBits(int const raw);
};

#endif
