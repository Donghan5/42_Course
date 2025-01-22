/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Convert.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 12:39:56 by donghank          #+#    #+#             */
/*   Updated: 2025/01/21 13:34:09 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Convert.hpp"

/*
	Exceptions
*/
const char	*ScalarConverter::ConvertUnknownExceptions::what(void) const throw() { return "Unknown type"; }

/*
	Constructor and destructor with copy assignement
*/
ScalarConverter::ScalarConverter(std::string argLine) {
	// std::cout << "Default constructor ScalarConverter has been called" << std::endl;
	this->_c = '\0';
	this->_n = 0;
	this->_d = 0.0;
	this->_f = 0.0f;
	this->_impos = false;
	setStr(argLine);
}

ScalarConverter::ScalarConverter(const ScalarConverter &rhs) {
	// std::cout << "Copy constructor of ScalarConverter has been called" << std::endl;
	*this = rhs;
}

ScalarConverter::~ScalarConverter() {
	// std::cout << "Destructor of ScalarConverter has been called" << std::endl;
}

/*
	@param
		rhs = copy assignement
*/
ScalarConverter	&ScalarConverter::operator=(const ScalarConverter &rhs) {
	if (this != &rhs) {
		this->_c = rhs._c;
		this->_n = rhs._n;
		this->_d = rhs._d;
		this->_f = rhs._f;
		this->_impos = rhs._impos;
		this->_str = rhs._str;
		this->_type = rhs._type;
	}
	return *this;
}

/*
	getter part
*/
char	ScalarConverter::getChar() const {
	return this->_c;
}

int	ScalarConverter::getInt() const {
	return this->_n;
}

double	ScalarConverter::getDouble() const {
	return this->_d;
}

float	ScalarConverter::getFloat() const {
	return this->_f;
}

std::string	ScalarConverter::getStr() const {
	return this->_str;
}

e_type	ScalarConverter::getType() const {
	return this->_type;
}

/*
	setter
*/
void	ScalarConverter::setInt(int n) {
	this->_n = n;
}

void	ScalarConverter::setDouble(double d) {
	this->_d = d;
}

void	ScalarConverter::setFloat(float f) {
	this->_f = f;
}

void	ScalarConverter::setChar(char c) {
	this->_c = c;
}

void	ScalarConverter::setStr(std::string str) {
	this->_str = str;
	this->setType();
	if (this->getType() == NONE) {
		throw ScalarConverter::ConvertUnknownExceptions();
	}
}

void	ScalarConverter::setType(void) {
	if (isChar())
		this->_type = CHAR;
	else if (isInt())
		this->_type = INT;
	else if (isDouble())
		this->_type = DOUBLE;
	else if (isFloat())
		this->_type = FLOAT;
}

/*
	bool functions
*/
/*
	verify int value
	@return
		boolean value;
*/
bool	ScalarConverter::isInt(void) const {
	int	j = 0;
	if (_str[j] == '+' || _str[j] == '-')
		j++;
	for (int i(j); i < (int)_str.length(); i++) { // direct init
		if (!std::isdigit(_str[i]))
			return false;
	}
	return true;
}

/*
	verify double value
	@param
		found = flag of .
	@return
		boolean value
*/
bool	ScalarConverter::isDouble(void) const {
	if (_str.find('.') == std::string::npos || _str.find('.') == 0 || _str.find('.') == _str.length() - 1)
		return false;
	int j = 0;
	int	found = 0;
	if (_str[j] == '+' || _str[j] == '-')
		j++;
	for (int i(j); i < (int)_str.length(); i++) {
		if (_str[i] == '.')
			found++;
		if ((!std::isdigit(_str[i])) && _str[i] != '.' && found > 1)
			return false;
	}
	return true;
}

/*
	verify float value
	@param
		found = flag of .
	@return
		boolean value
*/
bool	ScalarConverter::isFloat(void) const {
	std::string::size_type	dotPos = _str.find('.');
	std::string::size_type	fPos = _str.find('f');
	if (dotPos == std::string::npos || dotPos == 0 || fPos != _str.length() - 1)
		return false;
	int j = 0;
	int found = 0;
	if (_str[j] == '+' || _str[j] == '-')
		j++;
	for (int i(j); i < (int)_str.length(); i++) {
		if (_str[i] == '.')
			found++;
		if ((!std::isdigit(_str[i])) && _str[i] != '.' && found > 1)
			return false;
	}
	return true;
}

/*
	check the character
	@return
		true: if all conditions are true
		false
*/
bool	ScalarConverter::isChar(void) const {
	return _str.length() == 1 && std::isalpha(_str[0]) && std::isprint(_str[0]);
}

/*
	check the char is literal
	filter nan or infinity
	@return
		false: not literal
		true: literal func
*/
bool	ScalarConverter::isLiteral(void) const {
	if ((this->_impos) || (_str.compare("+inf") == 0) || (_str.compare("-inf") == 0) ||
		(_str.compare("+inff") == 0) || (_str.compare("-inff") == 0) ||
		(_str.compare("nan") == 0)|| (_str.compare("nanf") == 0))
			return true;
	return false;
}

/*
	check the possibility to convert
	@return
		impossible = true
		possible = false
*/
bool	ScalarConverter::isImpossible(void) {
	try {
		if (this->_type == INT)
			this->_n = std::atoi(_str.c_str());
		else if (this->_type == FLOAT)
			this->_f = std::atof(_str.c_str());
		else if (this->_type == DOUBLE)
			this->_d = std::strtod(_str.c_str(), NULL);
	}
	catch (const std::out_of_range &e) { // overflow and underflow
		this->_impos = true;
		return true;
	}
	catch (const std::exception &e) { // general exceptions
		this->_impos = true;
		return true;
	}
	// char verifications?
	return false;
}

/*
	Print function
	Reason to using _i --> convert to ascii number to verify printable
*/
void	ScalarConverter::printChar(std::ostream &out) const {
	if ((this->isLiteral()) || ((!std::isprint(this->_n)) && (this->_n) >= 127))
		out << "Impossible" << std::endl;
	else if (!std::isprint(this->_n))
		out << "Non displayable" << std::endl;
	else
		out << "'" << getChar() << "'" << std::endl;
}

void	ScalarConverter::printInt(std::ostream &out) const {
	if (this->isLiteral() || ((!std::isprint(this->_n)) && (this->_n) >= 127))
		out << "Impossible" << std::endl;
	else
		out << "'" << getInt() << "'" << std::endl;
}

void	ScalarConverter::printDouble(std::ostream &out) const {
	if ((_str.compare("nan") == 0) || (_str.compare("nanf") == 0))
		out << "nan" << std::endl;
	else if ((_str.compare("+inf") == 0) || (_str.compare("+inff") == 0))
		out << "+inf" << std::endl;
	else if ((_str.compare("-inf") == 0) || (_str.compare("-inff") == 0))
		out << "-inf" << std::endl;
	else if (_impos)
		out << "Impossible" << std::endl;
	else {
		if ((this->_d) - static_cast<int>(this->_d) == 0)
			out << getDouble() << ".0" << std::endl;
		else
			out << "'" << getDouble() << "'" << std::endl;
	}
}

void	ScalarConverter::printFloat(std::ostream &out) const {
	if ((_str.compare("nan") == 0) || (_str.compare("nanf") == 0))
		out << "nan" << std::endl;
	else if((_str.compare("+inf") == 0) || _str.compare("+inff") == 0)
		out << "+inf" << std::endl;
	else if ((_str.compare("-inf") == 0) || (_str.compare("-inff") == 0))
		out << "-inf" << std::endl;
	else if (_impos)
		out << "Impossible" << std::endl;
	else {
		if ((this->_f) - static_cast<int>(this->_f) == 0)
			out << getFloat() << ".0f" << std::endl;
		else
			out << "'" << getFloat() << "'" << std::endl;
	}
}

/*
	Converting function
	swith 구문에 인스턴스 객체를 사용할 수 없는 이유

*/
void	ScalarConverter::convert(void) {
	if (isImpossible())
		return ;
	switch (this->_type)
	{
		case CHAR:
			_c = _str[0];
			_n = static_cast<int>(_c);
			_d = static_cast<double>(_c);
			_f = static_cast<float>(_c);
			break;
		case INT:
			_n = std::atoi(_str.c_str());
			_c = static_cast<char>(_n);
			_d = static_cast<double>(_n);
			_f = static_cast<float>(_n);
			break;
		case DOUBLE:
			_d = std::strtod(_str.c_str(), NULL);
			_c = static_cast<char>(_d);
			_n = static_cast<int>(_d);
			_f = static_cast<float>(_d);
			break;
		case FLOAT:
			_f = std::atof(_str.c_str());
			_c = static_cast<char>(_f);
			_n = static_cast<int>(_f);
			_d = static_cast<double>(_f);
			break;
		default:
			break;
	}
}

std::ostream	&operator<<(std::ostream &out, const ScalarConverter &rhs) {
	out << "char: " ; rhs.printChar(out);
	out << "int: " ; rhs.printInt(out);
	out << "float: " ; rhs.printFloat(out);
	out << "double: " ; rhs.printDouble(out);

	return out;
}
