/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Form.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:51:45 by donghank          #+#    #+#             */
/*   Updated: 2024/12/22 00:51:46 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORM_HPP
#define FORM_HPP
#pragma once

#include "Bureaucrat.hpp"
#include <iostream>
#include <string>
#include <exception>

class Bureaucrat; // forward declaration

class Form {
	private:
		const std::string	_name;
		const int			_gradeToSign;
		const int 			_gradeToExec;
		bool				_signed;
		const static int	maxGrade = 1;
		const static int 	minGrade = 150;

	public:
		/* constructor with copy assignement and destructor */
		Form();
		Form(const std::string &name, int gradeToSign, int gradeToExec);
		Form(const Form &rightSide);
		Form &operator=(const Form &rightSide);
		~Form();

		/* basic functions */
		void				beSigned(const Bureaucrat &b);
		bool				isSigned(void) const;
		const std::string	&getName(void) const;
		int					getGradeToSign(void) const;
		int					getGradeToExec(void) const;

		/* Exception */
		class GradeTooLowException: public std::exception {
			public:
				const char	*what() const throw();
		};

		class GradeTooHighException: public std::exception {
			public:
				const char	*what() const throw();
		};
};

std::ostream	&operator<<(std::ostream &out, const Form &form);

#endif
