/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:15:35 by donghank          #+#    #+#             */
/*   Updated: 2024/11/28 12:39:39 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP
#pragma once

// standard lib(stream)
#include <iostream>
#include <string>
#include <cmath>

/*
	define the class
	Fixed value to binary
	Three part	- sign
				- Exponent : by using **bias** to control of the area of num
				- Fraction or Mantissa : store decimal part
	Converting decimal to binary --> 1. convert integer part
									 2. decimal multiple by 2, convert integer part
									 3. repeat this action
*/
class Fixed {
	private:
		int					_fixedPointVal;
		static const int	_fractionalBits = 8;

	public:
		Fixed();
		Fixed(const int n);
		Fixed(const float n);
		/*
			think *this (actuall status of object)
		*/
		Fixed(const Fixed &rightSide); // copy constructor
		Fixed &operator=(const Fixed &rightSide); // A copy assignment operator overload.
		~Fixed();

		int		getRawBits(void) const;
		void	setRawBits(int const raw);
		int		toInt(void) const;
		float	toFloat(void) const;
};

std::ostream & operator<<( std::ostream & o, Fixed const & i );

#endif
