/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bureaucrat.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:51:29 by donghank          #+#    #+#             */
/*   Updated: 2024/12/22 00:51:30 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP
#pragma once

#include <iostream>
#include <exception>
#include <string>

class Bureaucrat {
	private:
		const std::string	_name;
		int					_grade;
		static const int	maxGrade = 1;
		static const int	minGrade = 150;

	public:
		// constructor and desturctor
		Bureaucrat();
		Bureaucrat(const std::string &name, int grade);
		Bureaucrat(const Bureaucrat &rightSide);
		~Bureaucrat();

		// copy assignement
		Bureaucrat	&operator=(const Bureaucrat &rightSide);

		// Member function
		void	incrementGrade(void);
		void	decrementGrade(void);

		// getter
		const std::string	&getName(void) const;
		int					getGrade(void) const;

		// Exception
		class GradeTooHighException: public std::exception {
			public:
				const char *what(void) const throw();
		};

		class GradeTooLowException: public std::exception {
			public:
				const char *what(void) const throw();
		};
};

std::ostream	&operator<<(std::ostream &str, const Bureaucrat &bureaucrat);

#endif
