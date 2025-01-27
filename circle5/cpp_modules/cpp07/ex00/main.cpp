/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:31:46 by donghank          #+#    #+#             */
/*   Updated: 2025/01/09 23:31:50 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "whatever.hpp"

int main( void ) {
	{
		std::cout << "========== SUBJECT TESTS ==========" << std::endl;
		int a = 2;
		int b = 3;
		::swap( a, b );
		std::cout << "a = " << a << ", b = " << b << std::endl;
		std::cout << "min( a, b ) = " << ::min( a, b ) << std::endl;
		std::cout << "max( a, b ) = " << ::max( a, b ) << std::endl;
		std::string c = "chaine1";
		std::string d = "chaine2";
		::swap(c, d);
		std::cout << "c = " << c << ", d = " << d << std::endl;
		std::cout << "min( c, d ) = " << ::min( c, d ) << std::endl;
		std::cout << "max( c, d ) = " << ::max( c, d ) << std::endl;
	}
	{
		std::cout << "========== MY OWN TESTS ==========" << std::endl;
		int a = 30303030;
		int b = 39394;
		std::cout << "before swap" << std::endl;
		std::cout << "a = " << a << " / b = " << b << std::endl;

		swap(a, b);
		std::cout << "after swap" << std::endl;
		std::cout << "a = " << a << " / b = " << b << std::endl;
		std::cout << "min(a, b) = " << min(a, b) << std::endl;
		std::cout << "max(a, b) = " << max(a, b) << std::endl;

		std::string c = "hello1";
		std::string d = "hello2";
		std::cout << "before swap" << std::endl;
		std::cout << "c = " << c << " / d = " << d << std::endl;

		swap(c, d);
		std::cout << "after swap" << std::endl;
		std::cout << "c = " << c << " / d = " << d << std::endl;
		std::cout << "min(c, d) = " << min(c, d) << std::endl;
		std::cout << "max(c, d) = " << max(c, d) << std::endl;

		std::cout << "========== END TEST ==========" << std::endl;
	}
}

