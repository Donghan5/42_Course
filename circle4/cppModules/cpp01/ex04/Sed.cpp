/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sed.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:14:42 by donghank          #+#    #+#             */
/*   Updated: 2024/11/23 15:50:25 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sed.hpp"

/*
	constructor: init the infile name and define outfile name
*/
Sed::Sed(std::string filename): _inFile(filename) {
	this->_outFile = this->_inFile + ".replace";
	// std::cout << "Constructor of Sed has been called" << std::endl;
}

/*
	destructor
*/
Sed::~Sed(void) {
	// std::cout << "Destructor of Sed has been called" << std::endl;
}

/*
	implement replace function
	1. file open 2. file read 3. erase 4. input (3 and 4 replace)
	std::string::npos --> not found
*/
void	Sed::replace(std::string toFind, std::string toReplace) {
	std::ifstream	ifs(this->_inFile.c_str()); // str::string -> const char *
	if (ifs.is_open()) {
		std::string	content;
		if (std::getline(ifs, content, '\0')) {
			std::ofstream	ofs(this->_outFile.c_str());
			size_t	pos = content.find(toFind);
			// std::string::npos --> case: not found
			while (pos != std::string::npos) {
				content.erase(pos, toFind.length()); // delete the element toFind
				content.insert(pos, toReplace); // insert toReplace
				pos = content.find(toFind); // to increment the loop !!
			}
			ofs << content;
			ofs.close(); // close output stream
		}
		else {
			std::cerr << "empty file founded" << std::endl;
		}
		ifs.close(); // close input stream
	}
	else {
		std::cerr << "Fail to open the file" << std::endl;
		exit(EXIT_FAILURE);
	}
}
