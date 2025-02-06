/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 23:48:49 by donghank          #+#    #+#             */
/*   Updated: 2025/02/02 02:47:12 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

/*
	Exception part
*/
const char	*Rpn::InvalidInputException::what() const throw() {
	return "Error(Invalid input)";
}

const char *Rpn::CalculateException::what() const throw() {
	return "Error (Calculate)";
}

const char *Rpn::NotEnoughException::what() const throw() {
	return "Error (Not enough)";
}

const char *Rpn::RuleRpnException::what() const throw() {
	return "Error (Rule Rpn)";
}

/*
	To verify the operator
	@param
		op: operator (char to check)
	@return
		ascii value of each operators
*/
bool	Rpn::isOperator(char op) {
	return (op == '+' || op == '-' || op == '*' || op == '/');
}

/*
	To check the vaildate input
	@param
		line: input line to check the vaildity
*/
void	Rpn::validateInput(std::string line) {
	if (line.size() == 1 && (line[0] == '+' || line[0] == '-' || line[0] == '/' || line[0] == '*'))
		return ;
	char *endPtr = NULL; // to stock the pointer after convert
	double value = std::strtod(line.c_str(), &endPtr);
	if (value == 0.0 && !std::isdigit(line[0])) throw Rpn::InvalidInputException(); // to check 0.0 cases
	if (*endPtr && std::strcmp(endPtr, "f") != 0) throw Rpn::InvalidInputException(); // to check "12.4f" cases
	if (value < 0 || value >= 10) throw Rpn::InvalidInputException(); // to check out of range
	++this->numOfVal; // have to add...!
}

/*
	Doing calculate
*/
double	Rpn::doOperation(double a, double b, char op) {
	switch (op)
	{
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/':
			if (b == 0)
				throw Rpn::CalculateException();
			return a / b;
		default:
			throw Rpn::CalculateException();
	}
}

/*
	Split the input by white spaces
*/
void	Rpn::splitStrStock(void) {
	std::istringstream ss(this->_strArg);
	std::string stringBuf;
	std::stack<std::string> tmpStack;

	while (std::getline(ss, stringBuf, ' ')) { // split by white space
		if (stringBuf.empty()) continue ;
		tmpStack.push(stringBuf);
	}

	this->numOfVal = 0;
	while (!tmpStack.empty()) { // double verification of the white spaces during texts
		this->_splitStr.push(tmpStack.top());
		this->validateInput(tmpStack.top());
		tmpStack.pop();
	}
	// formula => num of val = num of operator + 1
	// _splitStr.size() = num of the value + operator
	if (2 * this->numOfVal - this->_splitStr.size() != 1) throw Rpn::RuleRpnException();
}

/*
	Calculate and store in the container
*/
void	Rpn::calculateStore(void) {
	std::string tmp;
	while (!this->_splitStr.empty()) {
		tmp = this->_splitStr.top();
		if (isOperator(tmp[0])) {
			if (this->_number.size() < 2) {
				throw Rpn::NotEnoughException();
			}
			double a, b; // poping the first 2 element (value) of the stack
			b = this->_number.top();
			this->_number.pop();
			a = this->_number.top();
			this->_number.pop();
			this->_number.push(doOperation(a, b, tmp[0])); // calculate and push to the number
			this->_splitStr.pop();
		} else {
			double value = std::strtod(this->_splitStr.top().c_str(), NULL); // convert the top element
			this->_number.push(value);
			this->_splitStr.pop();
		}
	}
	std::cout << _number.top() << std::endl; // showing top element
}

Rpn::Rpn(std::string strArg): _strArg(strArg), numOfVal(0) {} // constructor of type

Rpn::Rpn(const Rpn &rhs) { *this = rhs; } // copy constructor

// copy assignement
Rpn	&Rpn::operator=(const Rpn &rhs) {
	if (this != &rhs) {
		this->_number = rhs._number;
		this->_splitStr = rhs._splitStr;
		this->_strArg = rhs._strArg;
		this->numOfVal = rhs.numOfVal;
	}
	return (*this);
}

Rpn::~Rpn() {} // destructor

/*
	to print the result of the RPN equtation
*/
void	Rpn::printResultOfRpn(void) {
	try {
		splitStrStock();
		calculateStore();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
