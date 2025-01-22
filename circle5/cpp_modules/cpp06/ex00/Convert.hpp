/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Convert.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 12:27:07 by donghank          #+#    #+#             */
/*   Updated: 2025/01/21 13:24:36 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONVERT_HPP
#define CONVERT_HPP
#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

enum e_type {
	CHAR,
	INT,
	DOUBLE,
	FLOAT,
	NONE,
	LITERALS
};

class ScalarConverter {
	private:
		char		_c;
		int			_n;
		double		_d;
		float		_f;

		bool		_impos;
		std::string	_str;
		e_type		_type;

		ScalarConverter();

	public:
		ScalarConverter(std::string argLine);
		ScalarConverter(const ScalarConverter &rhs);
		~ScalarConverter();

		ScalarConverter	&operator=(const ScalarConverter &rhs);

		// getter
		int			getInt(void) const;
		double		getDouble(void) const;
		float		getFloat(void) const;
		char		getChar(void) const;
		std::string	getStr(void) const;
		e_type		getType(void) const;

		// setter
		void	setInt(int i);
		void	setDouble(double d);
		void	setFloat(float f);
		void	setChar(char c);
		void	setStr(std::string str);
		void	setType(void);

		// print
		void	printChar(std::ostream &out) const;
		void	printInt(std::ostream &out) const;
		void	printDouble(std::ostream &out) const;
		void	printFloat(std::ostream &out) const;

		// bool function
		bool	isInt(void) const;
		bool	isDouble(void) const;
		bool	isFloat(void) const;
		bool	isChar(void) const;
		bool	isLiteral(void) const;
		bool	isImpossible(void);

		// converter function
		void	convert(void);

		// exception
		class ConvertUnknownExceptions: public std::exception {
			public:
				virtual const char	*what() const throw();
		};

		// class ConvertOverflowExceptions: public std::out_of_range {
		// 	public:
		// 		virtual const char *what() const throw() { return "Overflow detected" ;};
		// };
};

std::ostream	&operator<<(std::ostream &out, const ScalarConverter &rhs);

#endif
