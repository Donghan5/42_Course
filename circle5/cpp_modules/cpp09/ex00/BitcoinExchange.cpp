/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:38:00 by donghank          #+#    #+#             */
/*   Updated: 2025/02/04 21:14:35 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#define WHITESPACE " \t\n\r"

// Exception
const char *Btc::FileOpenErrException::what() const throw() {
	return "Error: Cannot open file";
}

void	Btc::setData(std::string date, float value) {
	this->data.insert(std::make_pair(date, value));
}

/*
	static function to erase left white spaces
*/
static std::string	&eraseLeftSpaces(std::string &str) {
	str.erase(0, str.find_first_not_of(WHITESPACE));
	return str;
}

/*
	static function to erase right white spaces
*/
static std::string	&eraseRightSpaces(std::string &str) {
	str.erase(str.find_last_not_of(WHITESPACE) + 1);
	return str;
}

/*
	normalize line
	@param
		line = to be normalized
	@return
		normalized line
*/
std::string	Btc::normalizeLine(std::string line) {
	return eraseLeftSpaces(eraseRightSpaces(line));
}

std::map<std::string, float> Btc::getData() {
	return this->data;
}

// loading data.csv file
void	Btc::loadDataFromFile() {
	int	lineNum = 0;
	std::ifstream file("data.csv");
	if (!file.is_open()) {
		throw FileOpenErrException();
	}
	std::string line;
	while (std::getline(file, line)) {
		if (lineNum == 0) {
			lineNum = 1;
			continue ;
		}
		size_t pos = line.find(',');
		if (pos == std::string::npos) {
			std::cerr << "Invalid Format" << std::endl;
			continue;
		}
		std::string data = line.substr(0, pos);
		float value;
		try {
			value = std::atof(line.substr(pos + 1).c_str());
		} catch (std::exception &a) {
			(void)a;
			std::cerr << "Error: Could not parse value" << std::endl;
			continue ;
		}
		this->setData(data, value);
	}
	file.close();
}

// constructor
Btc::Btc(): readFlag(1) {
	try {
		Btc::loadDataFromFile();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		readFlag = 0;
	}
}

// copy constructor
Btc::Btc(const Btc &rhs) {
	*this = rhs;
}

// destructor
Btc::~Btc() {}

// copy assignement
Btc	&Btc::operator=(const Btc &rhs) {
	if (this != &rhs) {
		this->data = rhs.data;
		this->readFlag = rhs.readFlag;
	}
	return *this;
}


/*
	check the format of date
	@param
		infoDate: information of date
	@return
		true: validate date
		false: invalidate date
*/
bool	Btc::checkDate(std::string infoDate) {
	std::stringstream ss(infoDate);
	std::string yearStr, monthStr, dayStr;

	if (!std::getline(ss, yearStr, '-') || !std::getline(ss, monthStr, '-') || !std::getline(ss, dayStr)) {
		std::cerr << "Error: parsing date" << std::endl;
		return false;
	}
	int year, month, day;
	try {
		year = std::atoi(yearStr.c_str());
		month = std::atoi(monthStr.c_str());
		day = std::atoi(dayStr.c_str());
	} catch (const std::exception &e) {
		(void)e;
		std::cerr << "Error: parsing date" << std::endl;
		return false;
	}
	if (year < 2009 || year > 2022) { // in data.csv, the range of year is between 2009 and 2022
		return false;
	}
	if (month < 1 || month > 12) {
		return false;
	}
	if (day < 1 || day > 31) {
		return false;
	}
	if ((day == 31) && (month == 4 || month == 6 || month == 9 || month == 11)) {
		return false;
	}
	if (year % 4 == 0 && ((year % 100 != 0 ) || (year % 400 == 0))) {
		if (month == 2 && day > 29) { // check leap year
			return false;
		}
	} else if (month == 2 && day > 28) {
		return false;
	}
	return true;
}

/*
    Check if the given value is valid.
    @param
        infoVal: String to be validated.
    @return
        true - Valid value.
        false - Invalid value.
*/
bool	Btc::checkValue(std::string infoVal) {
	float value = std::atof(infoVal.c_str());
	if (value < 0) {
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	if (value > 1000) {
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}

/*
	Check the validity of the input file
*/
bool	Btc::checkValidity(std::string infoDate, std::string infoVal) {
	if (checkDate(infoDate) == false) {
		std::cerr << "Error: bad input =>  " << infoDate << std::endl;
		return false;
	}
	if (checkValue(infoVal) == false) {
		return false;
	}
	return true;
}

/*
	Read an input data
*/
void	Btc::readInput(std::string inputPath) {
	std::ifstream inputFile(inputPath.c_str());
	std::string	line;

	if (!inputFile.is_open()) {
		throw FileOpenErrException();
	}
	if (inputFile.peek() == std::ifstream::traits_type::eof()) {
		std::cout << "File reached eof of file is empty" << std::endl;
	}
	// index of line
	int lineNum = 0;
	while (getline(inputFile, line)) {
		if (lineNum == 0) {
			lineNum = 1;
			continue ;
		}

		std::stringstream ss(line);
		std::string date, value;

		if (!std::getline(ss, date, '|') || !std::getline(ss, value)) {
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		date = normalizeLine(date);
		value = normalizeLine(value);
		if (value.empty()) {
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}
		std::map<std::string, float>::iterator it = this->data.upper_bound(date);
		if (it == this->data.begin()) {
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}
		--it;
		if (!checkValidity(date, value)) {
			continue;
		}
		float convertValue = std::atof(value.c_str());
		std::cout << date << " => " << value << " = " << convertValue * it->second << std::endl;
	}
}
