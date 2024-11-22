/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Account.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 15:38:10 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 17:12:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Account.hpp"
#include <iostream>
#include <string>
#include <ctime>

/*
	variable initialize
*/
int	Account::_nbAccounts = 0;
int	Account::_totalAmount = 0;
int	Account::_totalNbDeposits = 0;
int	Account::_totalNbWithdrawals = 0;

/*
	define getter functions
*/
int	Account::getNbAccounts(void) {
	return (_nbAccounts);
}

int	Account::getTotalAmount(void) {
	return (_totalAmount);
}

int	Account::getNbDeposits(void) {
	return (_totalNbDeposits);
}

int	Account::getNbWithdrawals(void) {
	return (_totalNbWithdrawals);
}

/*
	Show the timestamp of account
	To fit the form of the timestamp
	the form of the timestamp
*/
void	Account::_displayTimestamp(void) {
	time_t		curr_time;
	struct tm	*tm_local;

	time(&curr_time);
	tm_local = localtime(&curr_time); // to get the times
	// year --> in computer the criteria --> 1900
	std::cout << "[" << (tm_local->tm_year + 1900) ;
	// month
	if (tm_local->tm_mon + 1 < 10)
		std::cout << "0";
	std::cout << (tm_local->tm_mon + 1);
	// day
	if (tm_local->tm_mday < 10)
		std::cout << "0";
	std::cout << tm_local->tm_mday << "_";
	// hour
	if (tm_local->tm_hour < 10)
		std::cout << "0";
	std::cout << tm_local->tm_hour;
	// min
	if (tm_local->tm_min < 10)
		std::cout << "0";
	std::cout << tm_local->tm_min;
	// sec
	if (tm_local->tm_sec < 10)
		std::cout << "0";
	std::cout << (tm_local->tm_sec) << "]";
}

/*
	display the all infos of the Account
	to get current time from time_t and time struct
	like timeval in C
*/
void	Account::displayAccountsInfos(void) {
	_displayTimestamp();
	std::cout << "Account: " << _nbAccounts << "; ";
	std::cout << "Total: " << _totalAmount << "; ";
	std::cout << "Deposits: " << _totalNbDeposits << "; ";
	std::cout << "Withdrawals: " << _totalNbWithdrawals << std::endl;
}

/*
	constructor
*/
Account::Account(int initial_deposit) {
	this->_accountIndex = _nbAccounts;
	_nbAccounts++;
	this->_amount = initial_deposit;
	_totalAmount += initial_deposit;
	this->_nbDeposits = 0;
	this->_nbWithdrawals = 0;
	_displayTimestamp();
	// print the informations
	std::cout << "index: " << this->_accountIndex << "; ";
	std::cout << "amount: " << this->_amount << "; ";
	std::cout << "created" << std::endl;
}

/*
	destructor
*/
Account::~Account() {
	std::cout << "index: " << this->_accountIndex << "; ";
	std::cout << "amount" << this->_amount << "; ";
	std::cout << "closed" << std::endl;
}

/*
	to make the deposits
	amount --> calculate
*/
void	Account::makeDeposit(int deposit) {
	int	prev_amount;

	prev_amount = this->_amount; // store status actual of the deposit
	this->_amount += deposit; // add deposit
	this->_nbDeposits++; // update times of deposit
	_totalAmount += deposit; // update total amount
	_totalNbDeposits++; // update total times of deposit

	// display the times
	_displayTimestamp();
	std::cout << "index: " << this->_accountIndex << "; ";
	std::cout << "prev_amount: " << prev_amount << "; ";
	std::cout << "amount: " << this->_amount << "; ";
	std::cout << "deposit: " << deposit << "; ";
	std::cout << "number deposits: " << this->_nbDeposits << std::endl;
}

/*
	To create the action of the withdrawal
*/
bool	Account::makeWithdrawal(int withdrawal) {
	_displayTimestamp();
	std::cout << "index: " << this->_accountIndex << "; ";
	std::cout << "prev_amount: " << this->_amount << "; ";
	if (this->_amount - withdrawal < 0) {
		std::cout << "\033[1;31mrefused!!\033[0m" << std::endl;
		return (false);
	}
	std::cout << withdrawal << std::flush;
	this->_nbWithdrawals++;
	this->_amount -= withdrawal;
	_totalAmount -= withdrawal;
	this->_totalNbWithdrawals++;
	std::cout << "amount: " << this->_amount << "; ";
	std::cout << "number deposits: " << this->_nbDeposits << std::endl;
	return (true);
}

/*
	To check amount of the account
*/
int	Account::checkAmount() const {
	return (this->_amount);
}

/*
	Showing the actual status of the account
*/
void	Account::displayStatus() const {
	std::cout << "index " << this->_accountIndex << "; ";
	std::cout << "amount: " << this->_amount << "; ";
	std::cout << "deposits: " << this->_nbDeposits << "; ";
	std::cout << "withdrawals: " << this->_nbWithdrawals << std::endl;
}
