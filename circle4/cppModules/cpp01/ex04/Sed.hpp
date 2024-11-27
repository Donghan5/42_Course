/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sed.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:46 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:14:47 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SED_HPP
# define SED_HPP

#pragma once
// standard lib
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

class Sed {

	private:
		std::string	_inFile;
		std::string	_outFile;

	public:
		Sed(std::string filename);
		~Sed();
	void	replace(std::string s1, std::string s2);
};

#endif
