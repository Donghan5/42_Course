/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:16:12 by donghank          #+#    #+#             */
/*   Updated: 2024/11/28 12:45:40 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

/*
	initializer lists:
		ClassName(parameters): member1(val1), member2(val2)...
	why? --> Efficiency
*/

/*
	Default constructor
*/
Fixed::Fixed(): _fixedPointVal(0) {
	// std::cout << "Default constructor called" << std::endl;
}

/*
	int constructor
	to convert fixed point value --> by doing shift by fractional bits
*/
Fixed::Fixed(const int n): _fixedPointVal(n << _fractionalBits) {
	// std::cout << "Int constructor called" << std::endl;
}

/*
	float constructor
	to convert fix using roundf(), n multiple (1 << fractional bits)
*/
Fixed::Fixed(const float n): _fixedPointVal(roundf(n * (1 << _fractionalBits))) {
	// std::cout << "Float constructor called" << std::endl;
}

/*
	Copy constructor
*/
Fixed::Fixed(const Fixed &rightSide) {
	// std::cout << "Copy constructor called" << std::endl;
	setRawBits(rightSide.getRawBits());
}

/*
	Copy assignment operator
*/
Fixed	&Fixed::operator=(const Fixed &rightSide) {
	// std::cout << "Copy assignment operator called" << std::endl;
	if (this != &rightSide) {
		this->_fixedPointVal = rightSide.getRawBits();
	}
	return (*this);
}

/*
	Destructor
*/
Fixed::~Fixed() {
	// std::cout << "Destructor called" << std::endl;
}

int	Fixed::getRawBits(void) const {
	return (this->_fixedPointVal);
}

void	Fixed::setRawBits(int const raw) {
	this->_fixedPointVal = raw;
}

/*
	int to fixed point value
*/
int	Fixed::toInt(void) const {
	return (this->_fixedPointVal >> _fractionalBits);
}

/*
	float to fixed point value
*/
float	Fixed::toFloat(void) const {
	float	floatVal;

	floatVal = static_cast<float>(this->getRawBits()) / (1 << _fractionalBits);
	return (floatVal);
}

// to put member of class to output stream
std::ostream & operator<<(std::ostream & o, Fixed const & i) {
	o << i.toFloat();
	return (o);
}

/*
	*this compare &rightSide
	comparison operators
*/
bool	Fixed::operator>(const Fixed &rightSide) const {
	return (this->getRawBits() > rightSide.getRawBits());
}

bool	Fixed::operator<(const Fixed &rightSide) const {
	return (this->getRawBits() < rightSide.getRawBits());
}

bool	Fixed::operator>=(const Fixed &rightSide) const {
	return (this->getRawBits() >= rightSide.getRawBits());
}

bool	Fixed::operator<=(const Fixed &rightSide) const {
	return (this->getRawBits() <= rightSide.getRawBits());
}

bool	Fixed::operator==(const Fixed &rightSide) const {
	return (this->getRawBits() == rightSide.getRawBits());
}

bool	Fixed::operator!=(const Fixed &rightSide) const {
	return (this->getRawBits() != rightSide.getRawBits());
}

/*
	arithmetic operators
*/
Fixed	Fixed::operator+(const Fixed &rightSide) const {
	return (Fixed(this->toFloat() + rightSide.toFloat()));
}

Fixed	Fixed::operator-(const Fixed &rightSide) const {
	return (Fixed(this->toFloat() - rightSide.toFloat()));
}

Fixed	Fixed::operator*(const Fixed &rightSide) const {
	return (Fixed(this->toFloat() * rightSide.toFloat()));
}

Fixed	Fixed::operator/(const Fixed &rightSide) const {
	return (Fixed(this->toFloat() / rightSide.toFloat()));
}

/*
	prefix, postifx increment and decrement
	with & --> prefix
	non --> postfix
*/

Fixed	&Fixed::operator++(void) {
	++this->_fixedPointVal;
	return (*this);
}

Fixed	Fixed::operator++(int) {
	Fixed	tmp(*this); // create copy of current object

	tmp._fixedPointVal = this->_fixedPointVal++;
	return (tmp);
}

Fixed	&Fixed::operator--(void) {
	--this->_fixedPointVal;
	return (*this);
}

Fixed	Fixed::operator--(int) {
	Fixed	tmp(*this); // create copy of current object

	tmp._fixedPointVal = this->_fixedPointVal--;
	return (tmp);
}

/*
	min and max function
*/
Fixed	&Fixed::min(Fixed &a, Fixed &b) {
	if (a.getRawBits() < b.getRawBits())
		return (a);
	return (b);
}

const Fixed	&Fixed::min(const Fixed &a, const Fixed &b) {
	if (a.getRawBits() < b.getRawBits())
		return (a);
	return (b);
}

Fixed	&Fixed::max(Fixed &a, Fixed &b) {
	if (a.getRawBits() > b.getRawBits())
		return (a);
	return (b);
}

const Fixed	&Fixed::max(const Fixed &a, const Fixed &b) {
	if (a.getRawBits() > b.getRawBits())
		return (a);
	return (b);
}
