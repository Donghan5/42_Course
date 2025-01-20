/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 00:51:55 by donghank          #+#    #+#             */
/*   Updated: 2024/12/27 12:32:56 by donghank         ###   ########.fr       */
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

class AForm {
	private:
		const std::string	_name;
		const int			_gradeToSign;
		const int 			_gradeToExec;
		bool				_signed;
		const static int	maxGrade = 1;
		const static int 	minGrade = 150;

	protected:
		void	checkExecutability(const Bureaucrat &executor) const;

	public:
		/* constructor with copy assignement and destructor */
		AForm();
		AForm(const std::string &name, int gradeToSign, int gradeToExec);
		AForm(const AForm &rightSide);
		AForm &operator=(const AForm &rightSide);
		virtual ~AForm();

		/* basic functions */
		void				beSigned(const Bureaucrat &b);
		bool				isSigned(void) const;
		std::string			getName(void) const;
		int					getGradeToSign(void) const;
		int					getGradeToExec(void) const;
		virtual void		execute(const Bureaucrat &executor) const = 0;

		/* Exception */
		class GradeTooLowException: public std::exception {
			public:
				const char	*what() const throw();
		};

		class GradeTooHighException: public std::exception {
			public:
				const char	*what() const throw();
		};

		class InvalidFormException: public std::exception {
			public:
				const char	*what() const throw();
		};
};

std::ostream	&operator<<(std::ostream &out, const AForm &aform);

#endif
