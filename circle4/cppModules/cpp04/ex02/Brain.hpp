/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Brain.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:04 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:23:05 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_HPP
#define BRAIN_HPP
#pragma once

#include <iostream>
#include <string>

class Brain {
	private:
		std::string		_ideas[100];
		unsigned int	_size;
	public:
		Brain();
		Brain(const Brain &rightSide);
		Brain	&operator=(const Brain &rightSide);
		~Brain();
		const std::string	&getIdeas(unsigned int idx) const;
		bool				addIdeas(std::string idea);
};

#endif
